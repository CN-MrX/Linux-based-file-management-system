# Linux-based-file-management-system
应用Linux系统进行网络编程，实现了Linux文件服务器和Linux客户端的开发。 当涉及到一个新的客户端(在ARM板上运行的客户端)时，主函数创建一个子进程，客户端接入后来实现对服务器上文件基本信息的读取；同时，用户也可以从服务器下载文件或上传文件至服务器。 该设计可以支持和管理多个客户端同时访问，而不受某个客户端退出的影响。