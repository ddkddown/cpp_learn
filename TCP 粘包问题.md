# TCP 粘包问题

- 固定长度的数据包

如，规定每个协议包长度为64字节，每次收满64个字节才拿出来解析，否则缓存起来。

优点：格式简单。

缺点：灵活性差，如果包内容不足规定长度，还要填充特殊信息：\0，如果包长度超过指定字节数，又要分包分片，增加额外处理逻辑。



- 以指定字符为包的结束标志

  当字节流遇到某个特殊字符时，就认为到包尾。如FTP协议，数据末尾加上\r\n代表一个包结束。不足之处在于如果数据包内部就包含特殊字符，则需要对改字符做一些转码转义操作。

- 包头+包体格式

  1. 收到数据。

  2. 当前数据包是否够一个包头大小？否则缓存当前数据退出流程。

  3. 解析包头中包含的描述包体长度的字段。

  4. 剩余字节数是否够一个包体大小?否则缓存当前数据退出流程。

  5. 从数据中取出一个完整的包（包头+包体）。

  6. 解析改包。

     包头定义

     ```
     //强制一字节对齐
     #pragma pack(push, 1)
     //协议头
     struct msg
     {   
         int32_t  bodysize;         //包体大小  
     };
     #pragma pack(pop)
     ```

     流程实现代码

     ```
     //包最大字节数限制为10M
     #define MAX_PACKAGE_SIZE    10 * 1024 * 1024
     
     void ChatSession::OnRead(const std::shared_ptr<TcpConnection>& conn, Buffer* pBuffer, Timestamp receivTime)
     {
         while (true) //当buffer中有多个包，在循环中可进行依次处理。
         {
             //不够一个包头大小
             if (pBuffer->readableBytes() < (size_t)sizeof(msg))
             {
                 //LOGI << "buffer is not enough for a package header, pBuffer->readableBytes()=" << pBuffer->readableBytes() << ", sizeof(msg)=" << sizeof(msg);
                 return;
             }
     
             //取包头信息, 此时不能直接将buffer中包头数据读出，因为之后要是不足一个包大小还需要放回去，过于麻烦。
             msg header;
             memcpy(&header, pBuffer->peek(), sizeof(msg));
     
             //包头有错误，立即关闭连接
             //注意包体大小，如果包体太大占用内存过多，会被系统杀死
             if (header.bodysize <= 0 || header.bodysize > MAX_PACKAGE_SIZE)
             {
                 //客户端发非法数据包，服务器主动关闭之
                 LOGE("Illegal package, bodysize: %lld, close TcpConnection, client: %s", header.bodysize, conn->peerAddress().toIpPort().c_str());
                 conn->forceClose();
                 return;
             }
     
             //收到的数据不够一个完整的包
             if (pBuffer->readableBytes() < (size_t)header.bodysize + sizeof(msg))
                 return;
     
             pBuffer->retrieve(sizeof(msg));
             //inbuf用来存放当前要处理的包
             std::string inbuf;
             inbuf.append(pBuffer->peek(), header.bodysize);
             pBuffer->retrieve(header.bodysize);          
             //解包和业务处理
             if (!Process(conn, inbuf.c_str(), inbuf.length()))
             {
                 //客户端发非法数据包，服务器主动关闭之
                 LOGE("Process package error, close TcpConnection, client: %s", conn->peerAddress().toIpPort().c_str());
                 conn->forceClose();
                 return;
             }              
         }// end while-loop
     }
     ```

     

###### 进阶做法：对包体内容进行压缩，减少网络传输中对带宽的占用。

