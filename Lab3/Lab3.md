# EE367L Lab 3 Simple file server and client

（Repo of lab notes: <https://github.com/duck8880/EE367L>）

​    


## Submission deadline

  - Feb 8, 2018 7:30 pm
  - We are scheduled to work on Lab 5 on Feb 8, so please try to complete lab 3 by the beginning of the next lab.
  - Each student must submit individually on Laulima
  - Refer to [Lab3 handout](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/d4d19636-a0e6-4b23-be7d-3e438392b486/EE367Lab3-v2.pdf) for Submission Instructions

    ​

## Office Hours
  Wednesdays: 10:30-11: 30 am, POST 214   

  Thursday: 10:15-11:15 am, POST 214

​    


## Lab Stages
- **Stage 0: Run client.c and server.c using your own port numbers.** 

  - Edit the port number in [**client.c**](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/e92b962a-6f04-47a7-91a4-a36045c8696d/client.c) and [**server.c**](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/3b552893-8336-4d52-b736-587a0b60d3c3/server.c). (Identical number in the two files)

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
     
- **Stage 1: Have the server execute “ls” whenever a client tries to connect to it. The “ls” should output to the console.**
  
  - Add `execl("/usr/bin/ls", "ls", (char *)NULL);` in the child process in server.c
  
  - Example is in [**ls367.c**](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/a1db3538-c01b-4a10-b2b7-360929b20cb8/ls367.c)
  
- **Stage 2: Have the output of “ls” is sent back to the client, where it is displayed.**  

  - In the child process, create a pipe and fork a grandchild process.
  
  - In the grandchild process, use **dup2()** to redirect output from stdout to the pipe and execute "ls" command.
  
  - In child process, read the pipe and send the output to the client.
  
  - Refer to [**pipe.c**](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/acd131e2-ee40-4048-a614-a8212e8f3571/pipe.c)
