This repository contains a c file that allows you to control a dps powersupply via Bluetooth from Linux

# Building

```
make
```

# Getting started

First connect your DPS powersupply via bluetooth. On a Raspberry Pi 3:

```
sudo bluetoothctl
# agent on
# default-agent
# scan on
```

After you find your `RuiDengDPS`, pair with the powersupply:

```
pair 00:BA:55:57:85:CE
```

The powersupply uses pincode `1234`. 

Then on the commandline, make the connection:

```
$ sudo rfcomm connect rfcomm0 00:BA:55:57:85:CE 1
Connected /dev/rfcomm0 to 00:BA:55:57:85:CE on channel 1
Press CTRL-C for hangup
```

Then use the compiled c file to set the voltage to 3.3 volt:

```
./dps-bt --baud 9600 --volt 330
```
