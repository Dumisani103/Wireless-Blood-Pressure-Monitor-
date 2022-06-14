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

To be able to capture data from the memory chip, the chip's address had to be known. As a result, the Arduino code in Figure 2 was used to scan for the memory chip's address. The results from running the Arduino code are shown in kkk, which indicated that the address of the machine is 50 hexadecimal, which is 80 in decimal.





