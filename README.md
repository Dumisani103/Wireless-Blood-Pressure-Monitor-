# Wireless Blood Pressure Monitor 
## Introduction 
Hypertension is a condition in which the force of the blood against the artery walls is too high. Usually, hypertension is defined as blood pressure above 140/90 and is considered severe if the pressure is above 180/120. It is the most common cause of heart attacks and strokes (‘High blood pressure (hypertension) - Treatment - NHS’, no date), (AHA, 2016). Therefore, monitoring blood pressure is key to prevent heart attacks and stroke. However, most blood monitoring devices can’t digitally store on a web server and provide the key meaning of blood pressure results. Thus, this project aimed at designing a medical device that can measure blood pressure and synchronize it to a database with a possible interpretation of the results. 

The designed system will ease the following: patients’ data management, generation of patients’ progress reports, interpretation of results, and improvement of patients’ health care.

## Implementation 
The figure below shows the setup of the proposed system. 
![image](https://user-images.githubusercontent.com/64351671/173663263-523745d6-992e-49ee-b7cb-0b88f81be3fd.png)

The Prototype consists of a stand-alone blood pressure monitor ([EW3106](https://www.amazon.co.uk/Panasonic-Diagnostec-EW3106-Pressure-Monitor/dp/B0009OGHRW)) and a wireless module. The wireless module in this prototype was configured to send data from the stand-alone blood pressure monitor to an online database through the internet. The difficulty, however, was capturing the data/reading from the machine. There were two possibilities for capturing data: one was to capture the data as it passed through the I2C bus, and the other way was to capture the data after it had been stored in the machine's memory. However, the latter could not be accomplished because it is difficult to capture data that is not addressed to you.

The memory chip of the stand-alone blood pressure monitor was identified as BR24L02W, and data is transmitted over the I2C bus. The memory's pin diagram is shown in Figure below. The pins of interest for data transmission were SDA and SCL. 

![image](https://user-images.githubusercontent.com/64351671/173666917-a035db80-5666-4957-8a95-a1de0253ad42.png)

To be able to capture data from the memory chip, the chip's address had to be known. As a result, the Arduino [code](https://github.com/Dumisani103/Wireless-Blood-Pressure-Monitor-/blob/25ee90a8af7a48a891122cc88df739e65055ab3a/chip_Identification.ino) used to scan for the memory chip's address. The results from running the Arduino code indicated that the address of the machine is 50 hexadecimal, which is 80 in decimal.

Following memory chip identification, the memory chip was connected to the wireless module. EP32 is the wireless module deployed in this prototype. Aside from being a wireless module, the ESP32 also serves as a microcontroller. As a result, it was used to facilitate data capture using the I2C communication protocol. Figure below shows the overall circuit of the ESP32 (wireless module) and the memory chip (BR24L02W) within the blood pressure monitor machine. Pins 21 and 22 of the ESP32 are connected to the memory chip's SDA (Data pin) and SCL (clock signal pin), respectively. A switch is placed between the ground pin (G) and enable pin (EN) of the EP32. The switch allows the user to activate the memory chip and begin transferring data. 

![image](https://user-images.githubusercontent.com/64351671/173669892-45f0b914-d984-4d90-982e-ae04297d27fc.png)
![image](https://user-images.githubusercontent.com/64351671/173670313-b64592d2-cdb2-4bbe-ae1d-4bde097b9bc2.png)

The following steps describe the algorithm used for data acquisition from the machine to the EP32 CHIP: 
- Checking for bus activity, that is, determining whether or not a link exists between the memory chip and the ESP32.
- Identifying the address location for the latest measurement in the memory.
- Reading data from the identified address location. 
- Temporarily storing the data on the ESP32’s memory. 
Once the data has been captured, it is transmitted to the online database using the POST request technique. The entire Arduino code for both transmission and data acquisition is included in [here]()






