# EE367L Lab 7 Switched LAN simulator – part 2

（Repo of lab notes: <https://github.com/duck8880/EE367L>）

​  
## Notes

  - Submission deadline: **May 3**, 2018. Show the TA your program on Lab.
  - You can also show me any improvements in Lab 5 on the day of May 3. However, the improvements after the original deadline have to be graded with 10% deduction.
  - Lab 9 report (Written report 4, Report on Labs 2, 5 and 7) is due on **May 3**, 2018
  - Lab 10 Evaluation of team members is due by the midnight of May 2, 2018. (If your team has four members, add one more column in the table.)
  - You can finally merge your files using GitHub and test the integrated program then.

​    
## Files 

  - **main.c**: the main routine.

    - **main()**:

      - Read network configuration file, create the lists of nodes and links and implement them.
      - Create the network nodes as children processes
      - Start the main routine for the manager
  - **main.h**
    - Types of packets are defined here. We may need to add more types.
  - **man.c**: the manager software including the main routine of the manager.

    - **man_main()**: Parse and execute the command enter by the user.

    - **file_upload()**: Send the command 'u' (upload) to a host, which will then send a file to another host.
  - **host.c**: the host node software including the main routine for the host.

    - **host_main()**: has a infinite loop, where it does the following in each pass

      1. Receive and execute a command from the manager.  
         This may create jobs, which are put in the job queue.
        
      2. Examine its incoming network link and convert a packet into a “job”, then put the job in a job queue.
      
      3. Take exactly one job from the job queue, and execute the job.  
         This may create more jobs, which are put in the job queue.
        
      4. Go to sleep for ten milliseconds.
  - **host.h**: defines data types

    - The enum type **host_job_type** is defined here. We may need to define more types of jobs in it.
  - **switch.c**: We will create the file to implement the switch node, which is similar to a host node but has no connection to the manager. Also, as the tasks is quite simple for a switch node, we don't have to implement the job queue but directly execute the tasks sequentially.

    - **switch_main()**: has a infinite loop, where it does the following in each pass

      1. Examine its incoming network link and get packets.
      2. Look up the destination node ID in the forwarding table, and record the port number and the source node ID if there is no entry for such port. 
      3. Forward the packet to the corresponding port if we know, or broadcast the packet.
      3. Go to sleep for ten milliseconds.


​  

## Issues (The same as Lab5)

- **host.c** : function **job_q_add()** (line 169)

  Add the following statement in the "then branch" (**After line 174**):

  ```c
  j->next = NULL;
  ```

  Without this statement, the jobs added to the queue may be lost.

- **net.c** : function **load_net_data_file()**

  In **line 461**, change the "`=`" to "`==`" in the following if statement:

  ```c
  if (node_type = 'H') {
  ```

- **Configuration file**

  Note that there is a restriction of node ID that the ID should be consecutive numbers starting from **0**, (function load_net_data_file(), from line 470 to line 474 in net.c).


  ​
## Assignments

  - **Improvement 1: Arbitrary Network Topologies**

    The network will generate spanning tree by exchanging tree packets, in which there are tree information such as RootID, RootDist, etc. The regular packets (non-tree packets) will be sent to tree links only. More detail are presented in the lab handout.
   
    To achieve the improvement, we need to:
   
    1. Define data structure for local tree information and tree packets.
    
       - The local tree information for swtiches include `localRootID`, `localRootDist`, `localParent`, `localPortTree[k]`. 
       
         We can define them as several variables or as a structure.
       
       - The tree packets contain the following fields: `packetRootID`, `packetRootDist`, `packetSenderType`, `packetSenderChild`. There field can be stored in the payload of the ordinary packets. 
       
         We can define a structure for the payload of tree packet in **main.h**, where the structure `packet` is defined. 
       
    2. Send tree packets regularly to its neighbours.
    
       The nodes will out the fields in the tree packets according to their local information, and then send the tree packets regularly with intervals of hundreds of milliseconds. Use a counter for timing in the main loop in the host and swtich nodes.  
      
       Both switches and hosts send tree packets, except that the `packetSenderType` are different, and that fields other than `packetSenderType` from a host node are insignificant as a host must be in the tree and can't be the root. 
      
       Changes should be made to **switch.c** and **host.c**, and possibly **packet.c** and their corresponding .h files.
      
    3. The switch who receives a tree packet update its local tree information according to the information in the tree packet.
    
       The algorighm is in the handout. The switches can update the forwarding tables by reference to the incoming tree packets as well.
      
       Changes should be made to **switch.c**.

    I have uploaded a configuration file [circles.config](https://github.com/duck8880/EE367L/blob/master/Lab7/circles.config) for the following topology, you may use it for testing the improvement 1. 

    <img src="https://github.com/duck8880/EE367L/blob/master/Lab7/Figure1.png" width="400">



  - **Improvement 2: DNS Server and Downloading** 

    There will be a domain name service (DNS) server. A host’s user can download a file from a remote host by using the remote host’s domain name.

    - Part 1 – Implementing a ***very simplified*** DNS server.

      Features of the DNS server:
        - It has physical ID 100.
        - It is attached to the network like a host node, using only one link to a switch node.
        - There is at most one DNS server in the network.
        - Only hosts can have domain names.
        - The manager system does not connect to the DNS server.
        - Allow a user from a host to register a domain name for the host. This is done through the manager system.

      <img src="https://github.com/duck8880/EE367L/blob/master/Lab7/Figure2.png" width="400">

      A host can do two types of operations with the DNS server:

        - Register Domain Name and Reply.

          To register, a host sends a packet to the DNS server which carries its domain name; The DNS server add/update the name to its naming table; The DNS server reply to the host.

        - Request Physical ID and Reply.

          A host sent a domain name to the DNS server through a request packet; The DNS server replies if the physical ID exists, and if it exists, the physical ID as well.  

          ​

      To achieve the improvement, we need to:

      1. Define more types of packets (in **main.h**): `PKT_DNS_REG`, `PKT_DNS_REG_REPLY`, `PKT_DNS_REQ`, `PKT_DNS_REQ_REPLY`.
      2. Modify **host.c** for sending and accepting the DNS packets.
      3. Add a new command to the system manager (in **man.c**): Specifying and registering a domain name of a host.
      4. Create the DNS server (implemented in **dns.c** and **dns.h**): The routine is similar to a switch node. Make it able to accept and reply the DNS packets.
      5. Create a configuration file which contains the DNS server node. Create a new node-type for the DNS server (e.g. 'D'), and modify the **net.c**.

      ​

    - Part 2 – Host can ping a remote host using the remote host’s domain name

      Upgrade the system so that a host can ping a remote host using the remote host’s domain name.

      The ping is basically a two-step process:

      - Get the remote host’s ID from the DNS server (send the `PKT_DNS_REQ` and take the ID from the `PKT_DNS_REQ_REPLY`)
      - Ping the remote host.

      To achieve the improvement, we need to:

      1. Add a new command to the system manager (in **man.c**): Ping by the domain name.
      2. Modify **host.c** so that it can conduct the two-step process of ping-by-domain-name.

      ​

    - Part 3 – Host can download a file from a remote host using the remote host’s domain name.

      Upgrade the system so that a host can download a file from a remote host using the remote host’s domain name.
      The download is basically a two-step process:

      - Get the remote host’s ID from the DNS server
      - Download file from the remote host.

      To achieve the improvement, we need to:

      1. Implement the file download function in Lab 5.
      2. Add a new command to the system manager (in **man.c**): download by the domain name.
      3. Modify **host.c** so that it can conduct the two-step process of download-by-domain-name.

