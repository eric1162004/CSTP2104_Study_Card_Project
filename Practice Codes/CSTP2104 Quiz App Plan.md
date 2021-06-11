## CSTP2104 Study Card App

#### App Features:
- Users can create sets of study cards 
- Users can review the items in the card set
- Each card consist of - keyword and definition
- Users can delete any card set
- Users can edit card set by adding or removing items

#### User Interface
- Main Window
  - show all the card sets (as a list)
  - contains buttons to delete/edit card sets
- Card Set Editor Window (in a child window)
  - set the title for the card set
  - add or remove card items
- Study Window (in a child windows)
  - users review the items in a card set
  - each card item will display in shuffled and rotation 
  - keyword will appear first
  - users can click a button to reveal the definition of the keyword
  - users can remove the card item in the current loop
  - the loop will continue until no cards is left
  - Users can exit the loop at any time

#### Complexity Considerations:
- Use separate threads to create child windows
  - this will allow users to create **multiple study windows**
- Filesystem & Interoperability 
  - Use C# DLL to storing and retrieving data
- Use resources to load app icon
- Use registry 
  - to store cardSetID, nameOfCardSet, lastAccessDateTime, creationDateTime?
- Download card sets over network using http (maybe?)

#### Tasks
- Set up Project
- Create the Main Window, Card Set Editor Window, Study Window
- Add Buttons, Icon, List Controls (display cardsets, cardItems)
- Code FilesystemManager using C# DDL
  - Each card set should be stored in a separate file
  - Get list of fileNames from the directory where the card set data is stored
  - Create new file (when creating a new card set)
  - Save Data to file 
  - Delete a file (when deleting a card set)
- Link C# DDL to C++ Dynamic Library
- Add Study Window logic
  - Create Study loop for the Study Window
  - Remove a card item for the study loop (when the user click on "Got it")
  - Reveal definition on the screen (when the user click on "Reveal")
  - Move to the next item (when the user click on "Next")