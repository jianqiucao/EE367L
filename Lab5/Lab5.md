# EE367L Lab 5 Switched LAN simulator – part 1

（Repo of lab notes: <https://github.com/duck8880/EE367L>）

​  
## Notes

  - Submission deadline: March 8, 2018. You have 4 - 5 weeks for Lab 5.
  
  - Lab 3 have a Late submission deadline on Feb. 15 with a deduction of 10%.
  
  - Lab 2 (Written report 1: Lifelong learning) is due on Feb 15. Submit the report in hard copy.
  
  - Form teams of three.
  
​  
## Assignments

  - Navigate the network simulator
  
    - Transfer LabSwitch.tar.gz and extract it using `tar -xvf LabSwitch.tar.gz`.
    
    - `make` it and run `./net367`
    
    - Navigate the command: s, m, h, c, ...

  - Improvement 1: File transfers
  
    - Improve the upload so that it can transfer files of at most 1000 bytes. 
      
      Break up a large file into multiple packets. The packets are transferred and reassembled at the destination host.
      
    - Implement a file download from another host, where a file can have up to 1000 bytes.
    
  - Improvement 2: Switch node
  
    - Create new files switch.c and possibly switch.h to implement the switch node.
    
  - Improvement 3: Sockets as a link option
  
    - Implement sockets to connect two subnetworks.
    
    - Extend the configuration file by adding socket links in it, e.g.,
    
      `S 4 wiliki.eng.hawaii.edu 3000 spectra.eng.hawaii.edu 3001`