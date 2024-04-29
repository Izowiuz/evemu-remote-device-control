## Prerequisites
---
### On the Host System

1. Ensure that SSH is installed.
2. Verify that sshpass is installed.

On the target system

1. Confirm that SSH is installed and running.
2. Install the evemu-event application.
3. Ensure that the user has appropriate access to /dev/event/* resources.

## Authorization
---

This application utilizes sshpass to relay commands to the target system. There are two methods of authorization:

1. Using a Password: This approach involves providing a password for authentication which is then kept in program memory.
2. Using SSH Keys (Private-Public Key Pair): **Option number 2 is recommended** due to its enhanced security. To use it, set the path to the private key and specify a program that will handle password prompts. Personally, I am using `ksshaskpass` which on my system is located under: `/usr/libexec/ssh/ksshaskpass`.
