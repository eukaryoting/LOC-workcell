MITOMI G2 V1.8
├── Main[OB1]  # Main functional block
│   ├── PWM Function[FC1]  # Control and mode switch of 4 PWM channels
│   ├── Cooling[FC2]  # Stop-go control of cooling
│   ├── Solenoid Update[FC4]  # Solenoid control in manual mode 
├── Cyclic Flowrate[OB30]   # Cyclic Interrupt block for PID control loop of pressure-flowrate and data transmission
└── Startup[OB100]  # Init value storage

#The details of OPC-UA interface to be found in Miro : https://miro.com/app/board/uXjVP7_18YY=/