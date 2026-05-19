/
├── include/
│   └── Config.h               // Chua cac define cong ket noi, hang so vat ly, he so sai so
├── lib/
│   ├── PathPlanner/           // (Nhiem vu 1 & 2) Dinh nghia duong ve, tinh toan vector
│   │   ├── PathPlanner.h
│   │   └── PathPlanner.cpp
│   ├── Kinematics/            // (Nhiem vu 3) Tinh toan hanh trinh, chuyen vector thanh goc Servo va bu tru sai so
│   │   ├── Kinematics.h
│   │   └── Kinematics.cpp
│   ├── CommandQueue/          // (Nhiem vu 4) Quan ly hang doi (Ring Buffer) de chong tran RAM
│   │   ├── CommandQueue.h
│   │   └── CommandQueue.cpp
│   └── HardwareController/    // (Nhiem vu 5) Xu ly tin hieu PWM, dieu khien Servo truc tiep
│       ├── HardwareController.h
│       └── HardwareController.cpp
├── src/
│   └── main.cpp               // Dieu phoi luong du lieu tu PathPlanner -> Kinematics -> Queue -> Hardware
├── test/
└── platformio.ini