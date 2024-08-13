Virtual Rocket Launch Timer

Introduction

This Software Requirements Document outlines the specifications for the timer functionality of the rocket launcher user interface (UI), incorporating UDP communication. The timer plays a critical role in ensuring precise synchronization and activation of rocket launch sequences, thereby enhancing operational efficiency and safety measures. This document details the functional and non-functional requirements necessary to achieve robust timer functionality, emphasizing seamless integration with UDP communication protocols to facilitate real-time data exchange between systems. By defining these requirements, we aim to ensure the reliable and accurate performance of the rocket launcher timer within the UI, meeting the operational needs and safety standards of our stakeholders.

Functional Requirements

Countdown Timer Functionality
1. Start Countdown: The application shall allow the user to start a countdown timer from a specified duration.
2. Stop Countdown: The application shall allow the user to stop the countdown timer, halting further countdown progress.
3. Reset Countdown: The application shall allow the user to reset the countdown timer to its initial duration.
4. Hold Countdown: The application shall allow the user to hold the countdown timer at its current elapsed time and communicate this held time to a client application.
   
UDP Communication:

UDP uses IP to get a datagram from one computer to another. UDP gathers data in a UDP packet and adds its own header information to the packet. The steps of establishing a UDP socket communication on the server side are as follows:
UDP Server :
1.	Create a UDP socket.
2.	Bind the socket to the server address.
3.	Wait until the datagram packet arrives from the client.
4.	Process the datagram packet and send a reply to the client.
5.	Go back to Step 3.
UDP Client :
1.	Create a UDP socket.
2.	Send a message to the server.
3.	Wait until a response from the server is received.
4.	Process the reply and go back to step 2, if necessary.
5.	Close socket descriptor and exit.

Non-Functional Requirements
- The application shall consist of two main components:
- Server Side: Implements the countdown timer and UDP server to handle client requests.
- Client Side: Receives held time updates from the server via UDP.

Performance
- The countdown timer shall be accurate to within one second.
- The application shall handle UDP communication efficiently with minimal latency.

Reliability
-The application shall handle errors gracefully, providing appropriate error messages or fallback actions when necessary.
Usability
- The user interface for controlling the countdown timer shall be intuitive and responsive.
UI Design
- The application shall consist of two main components:
  - Server Side: Implements the countdown timer and UDP server to handle client requests.
  - Client Side: Receives held time updates from the server via UDP.


Implementation Details
- Programming Language: c++
- Application used:Qt Software

User Scenarios

1: Start Countdown

  - Description: User starts a countdown timer for 60 seconds.
  - Steps:
    1. User sends a "start" command to the application.
    2. Application begins counting down from 60 seconds.
    3. Countdown updates are displayed on the server side.

 2: Hold Countdown
  - Description: User holds the countdown timer at 30 seconds and receives held time on the client side.
    1. User sends a "hold" command to the application.
    2. Application holds the countdown timer at 30 seconds.
    3. Held time (30 seconds) is sent to the client via UDP communication.
    4. Client receives and displays the held time.	
3:StopButton
The stop button halts the timer and interrupts any ongoing countdown or timing process. When a user clicks the stop button in the UI:
1.	UDP Communication: The UI sends a UDP message to the backend or control system indicating the request to stop the timer.
2.	Timer Halt: Upon receiving the UDP message, the backend immediately stops decrementing the timer or halts any ongoing countdown sequence.
3.	Feedback to UI: The backend sends a UDP acknowledgment (ACK) message back to the UI to confirm that the timer has been successfully stopped.
4.	UI Update: The UI updates the display to reflect the stopped timer state, possibly showing the elapsed time at the moment of stopping.
4:Reset Button:
The reset button initializes the timer to its initial state or a predefined starting point, readying it for a new countdown sequence. Here’s how it functions:
1.	UDP Communication: Similar to the stop button, clicking the reset button triggers a UDP message from the UI to the backend.
2.	Timer Reset: Upon receiving the reset command via UDP, the backend resets the timer value to zero or a specified starting time.
3.	Feedback to UI: The backend sends an ACK message to the UI confirming the reset operation.
4.	UI Update: The UI updates its display to show the timer reset to the initial state.indicating readiness for a new timing operation.

