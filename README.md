```
         __             //___  _____
    ____/ /_____ _____ // __ \/ ___/
   / __  // ___// ___/// / / /\__ \ 
  / /_/ /(__  )(__  )// /_/ /___/ / 
  \__,_//____//____///\____//____/  

The Dollar Store Shim Operating System
                                 
```

dssOS is an alternative operating system for Chrome OS devices which runs off a modified RMA shim.

> [!WARNING]  
> Warning: dssOS was hacked together in a weekend, and thus is very unstable. Don't come crying to me when something breaks.

## How to install

See [here](https://dssos.nineeleven.tech/#instructions)

## Where to get RootFSes

[Here](https://github.com/BomberFish/dssOS-rootFS/releases). Extract layer.tar in `basesystem`, making sure to keep a file structure similar to the following:

```
basesystem
└── layer
    ├── afs
    ├── bin -> usr/bin
    ├── boot
    ├── dev
    ├── etc
    ├── home
    ├── lib -> usr/lib
    ├── lib64 -> usr/lib64
    ├── lost+found
    ├── media
    ├── mnt
    ├── opt
    ├── proc
    ├── root
    ├── run
    ├── sbin -> usr/sbin
    ├── srv
    ├── sys
    ├── tmp
    ├── usr
    └── var
```

## Credits

Thanks to the sh1mmer project for providing some utilities used in the patcher script, as well as doing the hard work of figuring out what the hell the shims even do.