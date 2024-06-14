#include <LiquidCrystal_I2C.h>

#define MAX_MESSAGES 10
#define MESSAGE_LENGTH 32 // maximum length of each message, assuming 16 chars per line

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows

String messages[MAX_MESSAGES];
int messageCount = 0;
int currentMessageIndex = 0;

void setup() {
  lcd.begin(); // initialize the lcd
  lcd.backlight();
  Serial.begin(9600); // initialize serial communication at 9600 bps
}

void loop() {
  if (Serial.available() > 0) { // check if data is available to read
    String message = Serial.readStringUntil('\n'); // read the incoming message until a newline character

    if (message.startsWith("#CLEAR")) {
      clearMessages();
    } else if (message.endsWith("#DELETE")) {
      int index = message.substring(0, message.length() - 7).toInt();
      deleteMessage(index);
    } else if (message.endsWith("#EDIT")) {
      int index = message.substring(0, message.indexOf("#EDIT")).toInt();
      String newMessage = message.substring(message.indexOf("#EDIT") + 5);
      editMessage(index, newMessage);
    } else if (messageCount < MAX_MESSAGES) {
      messages[messageCount] = message;
      messageCount++;
    }
  }

  if (messageCount > 0) {
    displayMessage(messages[currentMessageIndex]);
    delay(2000); // wait for 2 seconds before showing the next message
    currentMessageIndex = (currentMessageIndex + 1) % messageCount; // move to the next message
  }
}

void displayMessage(String message) {
  lcd.clear(); // clear display
  lcd.setCursor(0, 0); // move cursor to (0, 0)

  if (message.length() <= 16) {
    lcd.print(message); // print the message on the first line if it's short enough
  } else {
    lcd.print(message.substring(0, 16)); // print the first 16 characters on the first line
    lcd.setCursor(0, 1); // move cursor to (0, 1)
    lcd.print(message.substring(16, 32)); // print the next 16 characters on the second line
  }
}

void clearMessages() {
  for (int i = 0; i < MAX_MESSAGES; i++) {
    messages[i] = "";
  }
  messageCount = 0;
  currentMessageIndex = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Messages Cleared");
  delay(2000);
  lcd.clear();
}

void deleteMessage(int index) {
  if (index >= 0 && index < messageCount) {
    for (int i = index; i < messageCount - 1; i++) {
      messages[i] = messages[i + 1];
    }
    messages[messageCount - 1] = "";
    messageCount--;
    if (currentMessageIndex >= messageCount) {
      currentMessageIndex = 0;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Message Deleted");
    delay(2000);
    lcd.clear();
  }
}

void editMessage(int index, String newMessage) {
  if (index >= 0 && index < messageCount) {
    messages[index] = newMessage;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Message Edited");
    delay(2000);
    lcd.clear();
  }
}
