# EE367L Lab 3 Simple file server and client

（Repo of lab notes: <https://github.com/duck8880/EE367L>）

​    


## Notes

  - Submission deadline is on Feb 8, 2018, 7:30 pm. **Late submission deadline on Feb. 15 with a deduction of 10%.**
  - We are scheduled to work on Lab 5 on Feb 8, so please try to complete lab 3 by the beginning of the next lab.
  - Each student must submit individually on Laulima. Refer to [Lab3 handout](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/d4d19636-a0e6-4b23-be7d-3e438392b486/EE367Lab3-v2.pdf) for Submission Instructions.
  - You can login to wiliki in multiple windows, in which you can run client and server separately, and even edit multiple files at the same time.

    ​

## Office Hours
  Wednesdays: 10:30-11:30 am, POST 214   

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

  - To check which process is using a specific port, you can use:

     `lsof -i :portNumber`

- **Stage 1: Have the server execute “ls” whenever a client tries to connect to it. The “ls” should output to the console.**

  - Add `execl("/usr/bin/ls", "ls", (char *)NULL);` in the child process in **server.c**

  - Example is in [**ls367.c**](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/a1db3538-c01b-4a10-b2b7-360929b20cb8/ls367.c)

- **Stage 2: Have the output of “ls” is sent back to the client, where it is displayed.**  

  - After accepted the connection from the client, the server create a child process to deal with the command.
  
  - In the child process, create a pipe and fork a grandchild process.

  - In the grandchild process, use **dup2()** to redirect output from stdout to the pipe and execute **"ls"** command.

  - In the child process, read the output from the pipe into a buffer, and send it to the client.

  - Just need to change **server.c**. Refer to [**pipe.c**](https://laulima.hawaii.edu/access/content/attachment/MAN.80605.201830/Assignments/acd131e2-ee40-4048-a614-a8212e8f3571/pipe.c).

- **Stage 3: The client should have a user interface that accepts the commands to “list” or “quit”.**

  - In **client.c**: 

    - Add **while()** loop for getting user input continuously.

    - Add code for parsing the command of "**l**" or "**q**".

    - send the command to the server using function **send()**.

  - In **server.c**: 
  
    - Receive the command coming from the server using function **recv()**. 
    
    - Parse the command, and execute **"ls"** if it is **"l"**.   
    
    Tip: **recv()** doesn't add '\0' to the end of the received string, so you have to at '\0' manually.

- **Stage 4: The client and server should include the command “check”.**

  - In **client.c**, add code for parsing "**c**". 

    - Tip: `scanf("%s", cmdline)` doesn't work for getting the whole line of user input. The input string will be cut off at the delimiters (e.g., space). So use the following code instead:

      ``` c
      fgets(cmdline, MAXDATASIZE, stdin);
      cmdline[strlen(cmdline) - 1] = '\0';
      ```

    - Tip: For separating command name ("c") and the file name from the command line, you can use strtok_r() or use your approach.

  - In **server.c**, add code for parsing "c" and checking the existence of a file. You can use "access(filename, F_OK)" for checking. The return value of 0 indicates that the file exists.

- **Stage 5: The client and server should include the command “display”.** 

  To implement “display”, first have the server display the file directly to the console. Then have the server send the file back to the client.

  - In **client.c**, add code for parsing "**p**" and displaying the content of the packet coming from the server.

  - In **server.c**, if a command display is received, execute "**cat**" command to show the content of the file, which is similar to **stage 2**, and then send the output to the client. Reading the content of the file out by your written code is also applicable.

- **Stage 6: The client and server should include the command “download”.**

  Note that “download” is different from “display” because the client will store the file rather than display it on the console.
  
  - In **client.c**:

    - Add code for parsing "**d**". Check the existence of the file. If it exists, query user for if it would like to overwrite the file before send the command to the server.

    - Create the file by using `FILE *fp = fopen(file_name, "w");`, and write file using `fputs(buf, fp);`  
   
  - In **server.c**, check if the file exists first, and send a message to let the client know the existence. If it exists, send the content to the client.    
  
    Tip: You can run client367 in another directory so that the client won't overwrite the original file for downloading it.

- **Stage 7: Complete the assignment.**

  Implement command "**h**" in the client.




## Advanced Task

Deal with the problem that the file may be too large to fit into one packet in transmission when executing the "p" or "d" command (e.g., for file3.txt). You need to read the file several times and transmit it thru multiple packets.  
  
There are several ways to deal with multiple-packet transmission between server and client: 

- Server calculates #packet and tells client before the beginning of the transmission. The server stops after sent sufficient #packets, and the client stops received sufficient #packets. 

- Add header in the packets to tell if the packet is intermediate packet or the last packet.

- Add control character(e.g., EOT, '\x04') to show the end of the last packet.
