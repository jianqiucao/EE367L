# EE367L Lab 3 Simple file server and client

（Repo of lab notes: <https://github.com/duck8880/EE367L>）

​    


## Submission deadline

  - Feb 8, 2018 7:30 pm
  - We are scheduled to work on Lab 5 on Feb 8, so please try to complete lab 3 by the beginning of the next lab.

    ​

## Office Hours
  Wednesdays: 10:30-11: 30 am, POST 214   

  Thursday: 10:15-11:15 am, POST 214

​    


## Lab Stages
- Simple test

  - Edit the port number in **client.c** and **server.c**. (Identical number in the two files)

  - Write a **makefile** and make the **client.c** and **server.c** into executables **client367** and **server367**.

  - Run the server in background:   

     `./server367 &`  

  - Run the server: 

    `client367 wiliki.eng.hawaii.edu`

  - You'll get the display:

    ```bash
    client: connecting to 127.0.0.1
    server: got connection from 127.0.0.1
    client: received 'Hello, world!'
    ```

  - Kill the server:

     `fg`  
     `Crtl-C`

