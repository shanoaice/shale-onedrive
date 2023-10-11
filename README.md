# shale-onedrive

> 🚧 **This is a work in progress...** 🚧

Mount your Microsoft OneDrive Storage as a FUSE filesystem (with layering that prevents accidental deletion of remote files).

## Install

### AUR

### Build from Source

#### Prerequsites
- C++ compiler with C++20 support
- meson **and** CMake
- boost
- mimalloc
- simdjson(*)
- cpr(*)
- yyjson(*)

(*) optional, will be downloaded by Meson if not installed on local machine

## Usage

> _Obtain Credentials (WIP)_

Then, create two empty directories to mount two layers of storage:

```sh
mkdir -p ~/onedrive-working/{upper,lower}
shale mount --lower ~/onedrive-working/lower
shale mount --upper ~/onedrive-working/upper
```

After that, combine the two filesystems using overlayfs (require kernel module `overlay` if you decide to compile your own kernel):

```sh
mkdir ~/onedrive
mount -t overlay overlay -o lowerdir=~/onedrive-working/lower,upperdir=~/onedrive-working/upper/current,workdir=~/onedrive-working/upper/work ~/onedrive
```

And you are done. Files will be pulled from the cloud only when needed. 

This setup gives you an advantage:
- Deleting files in the overlayfs will only delete the locally avaliable copy, to prevent any data loss.
- If you want to delete a file completely (both locally and remotely), `rm` corresponding files in the lower directory (in this case, `~/onedrive-working/lower/`).
- In any other circumstances, do not access files in the lower or upper filesystem directly to prevent any unintended data loss.

### Unmount the filesystem

To unmount the file system, **DO NOT** kill the `shale` instance, as this can very possibly cause data loss.

If you are using the simple setup, simply `fusermount -u` the OneDrive directory.

If you are using the layered setup:
```sh
umount ~/onedrive # unmount the overlayfs first
fusermonut -u ~/onedrive-working/upper
fusermount -u ~/onedrive-working/lower # umount both FUSE layers
```
