## First Module

 Yaniv is using mint cinnamon linux
and Lubaba using a clean ubunto image

from:
sysprog21.github.io/lkmpg

```bash
sudo apt-get update
```

```bash
sudo apt-get install kmod linux-headers-5.4.0-80-generic
```

it was needed to restart the vm after installing the headers for linux/init
to not throw compiliation error

created the make file with the given instruction

```bash
obj-m += first_module.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

than did:

```bash
sudo insmod first_module.ko
```

```bash
dmesg | tail
```

[ 1026.162937] Basic Module: Loaded successfully!

```bash
sudo rmmod first_module
```

[ 1141.683887] Basic Module: Unloaded successfully!

now we can clean that make file cus it really disgusting

```bash
make clean
```

in order to push in this linux i must setup:
 git config --global user.email "you@example.com"
  git config --global user.name "Your Name"