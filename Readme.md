# kprobe kernel module

hooks on a given function.

Attention: was tested only with kernel version 5.13.0-51 (might do not work with other versions)

## how to build

```
make 
```

clean with

```
make clean
```


## how to (un)install:
uninstalling the kernel module:

```
make remove
```

installing the kernel module:

```
make install
```

or like that:


```
sudo insmod myprobe.ko hook="vfs_open"
```

replace "vfs_open" with the function name you want to hook on


watch the logs with

```
dmesg -w
```
