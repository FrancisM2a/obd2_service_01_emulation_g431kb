# OBD-II Service/Mode 01 emulation (STM32G431KB)
On-board diagnostic (OBD) scanners are another example of CAN devices you can play with at your home lab. After hacking a couple of IPCs let's do something potentially more productive :wink: Communicating with IPCs is mostly a tedious guessing and checking game. In contrast to that, the on-board diagnostics is a standardized communication between a vehicle and a scanner. You can use the standard to develop your own scanner. However, it takes two to communicate. And in such a scenario the other party is the vehicle itself - not a cheap option to perform tests. Do we then have to buy a car to experiment with the idea? Fortunately not. We can pose another problem that will make us try our hand on the standard. Let's buy a basic scanner for around $10 and emulate the vehicle. The SAE standard defines several services. Service 01 streams current sensor live data from PIDs (parameter IDs). This is probably the easiest service to emulate and thus we will choose it for starters. The exemplary code does not implement the full service 01 - just a couple of hard-coded PIDs from two different groups.

![OBD Service 01 in action](/Assets/Images/obd_service_01_in_action.jpg)

> [!IMPORTANT]
> Not all scanners are compatible with the iOS iPhones. The key recommendation here is to go for BT 4.x[^1]. For example, [Vgate iCar Pro BT4.0](https://vgate.pl/product/vgate-icar-pro-bt4-0/) can communicate with an iOS device, whereas its cheaper brother [Vgate iCar Pro BT3.0](https://vgate.pl/product/vgate-icar-pro-bt3-0/) fails to do that.

[^1]: [Bluetooth](https://en.wikipedia.org/wiki/Bluetooth) (Wikipedia)

# Missing files?
Don't worry :slightly_smiling_face: Just hit Alt-K to generate /Drivers/CMCIS/ and /Drivers/STM32G4xx_HAL_Driver/ based on the .ioc file. After a couple of seconds your project will be ready for building.

# Scanners in my lab
* [Vgate iCar2 BT3.0](https://vgate.pl/product/vgate-icar2-bt3-0/)
* [Vgate iCar Pro BT4.0](https://vgate.pl/product/vgate-icar-pro-bt4-0/)
* [Surix T31](https://allegro.pl/oferta/tester-diagnostyczny-interfejs-obd2-jezyk-polski-skaner-bledow-check-engine-14433734878)

# Pre-workshop materials
* [OBD2 Explained - A Simple Intro [2024]](https://www.csselectronics.com/pages/obd2-explained-simple-intro) (CSS Electronics)
* [On-board diagnostics](https://en.wikipedia.org/wiki/On-board_diagnostics) (Wikipedia)
* [OBD-II PIDs](https://en.wikipedia.org/wiki/OBD-II_PIDs) (Wikipedia)
* [OBD2 PID Overview [Lookup/Converter Tool, Table, CSV, DBC]](https://www.csselectronics.com/pages/obd2-pid-table-on-board-diagnostics-j1979) (CSS Electronics)

# ELM327
* [Original ELM327 OBD2 interfaces](https://carvitas.com/blog-and-news/original-elm327-obd2-interfaces)
* [OBDLink](https://www.obdlink.com/)
* [ELMduino](https://github.com/PowerBroker2/ELMduino)
* [Car Scanner ELM OBD2](https://play.google.com/store/apps/details?id=com.ovz.carscanner)

# Call for action
Create your own [home laboratory/workshop/garage](http://ufnalski.edu.pl/control_engineering_for_hobbyists/2024_dzien_popularyzacji_matematyki/Dzien_Popularyzacji_Matematyki_2024.pdf)! Get inspired by [ControllersTech](https://www.youtube.com/@ControllersTech), [DroneBot Workshop](https://www.youtube.com/@Dronebotworkshop), [Andreas Spiess](https://www.youtube.com/@AndreasSpiess), [GreatScott!](https://www.youtube.com/@greatscottlab), [ElectroBOOM](https://www.youtube.com/@ElectroBOOM), [Phil's Lab](https://www.youtube.com/@PhilsLab), [atomic14](https://www.youtube.com/@atomic14), [That Project](https://www.youtube.com/@ThatProject), [Paul McWhorter](https://www.youtube.com/@paulmcwhorter), [Max Imagination](https://www.youtube.com/@MaxImagination), [Nikodem Bartnik](https://www.youtube.com/@nikodembartnik), and many other professional hobbyists sharing their awesome projects and tutorials! Shout-out/kudos to all of them!

> [!WARNING]
> Control engineering - do try this at home :sunglasses:

190+ challenges to start from: [Control Engineering for Hobbyists at the Warsaw University of Technology](http://ufnalski.edu.pl/control_engineering_for_hobbyists/Control_Engineering_for_Hobbyists_list_of_challenges.pdf).

Stay tuned!
