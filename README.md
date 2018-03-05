# Test-SSL-server-with-WolfSSL

MultiThreaded Socket Progrm for testing Large SSL Network

A simple SSL server built with WolfSSL library to simulate large number of hosts.


There are 3 separate codebases

1. Default with WolfSSL Library
2. SSL Server with OpenSSL Library ( Huge memory consumption )
3. Default with WolfSSL Library and csv input.

Steps

1. Download WolfSSL library from WolfSSL site and extract into a folder of current project
2. Modify setup.sh to use the new WolfSSL library and execute setup.sh
3. Once WolfSSL library is compiled and ready
4. Modify start.sh to configure SSL servers on particular host and port ( IP should be given in IP Integer format for the program)
5. Run start.sh


**Note: On the client side need to add a static route to the configured host
