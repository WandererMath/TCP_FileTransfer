# TCP_FileTransfer

C++/C TCP socket programming to transfer files.

Large files are sliced into chunks to transfer. The program checks how many bytes are successfully transferred and resends if it failed to send.  
