# ![](https://forum.choiceofgames.com/uploads/choiceofgames/270/107efc2878dfc5fc.png) Chronicler  
**Description:**  
Chronicler is a cross-platform visual code editor for [ChoiceScript](https://www.choiceofgames.com/make-your-own-games/choicescript-intro/). It allows both beginners and veteran novelists alike to design interactive fiction with a flowchart-like layout rather than typing code in a text editor.

Forum post and pre-built binaries may be found [here](https://forum.choiceofgames.com/t/tool-chronicler-choicescript-visual-code-editor).

**Technologies:**  
C++, Qt

**Screenshots:**  
[Choice of the Dragon](https://www.choiceofgames.com/dragon/) example ChoiceScript game.  
[Example Chronicler project](https://www.dropbox.com/s/wcel02voz0i6ugt/Dragon.chronx?dl=1)  
![](https://www.dropbox.com/s/78gwnx2c27oa5bv/Chronicler2_052616_1.png?dl=1)  
![](https://www.dropbox.com/s/02mpwb34y8mm8dp/Chronicler2_052616_2.png?dl=1)  
![](https://www.dropbox.com/s/hnsunjcawrp6s0x/Chronicler2_052616_3.png?dl=1)  

**Installation:**  
1. Install Qt Creator (5.5 *not* 5.6) with the MinGW 32bit compiler.  
2. Import ChroniclerQT project  
3. Configure kits to use MinGW 32bit instead of MSVC.  
4. Build->Release  

**Contribution Guidelines:**  
Naming convention: Prefix all .h .cpp files with "C"

-- ***NOTE:*** *TODO*, *BUGS*, and *COMPLETE* sections moved to https://github.com/GarrettFleischer/ChroniclerQT/issues

**Bug reports** or **Feature requests** may be submitted here: https://gitreports.com/issue/GarrettFleischer/ChroniclerQT

**Contributors:**  
[Garrett Fleischer](https://www.linkedin.com/in/garrett-fleischer-57a230b7)


**CONTROLS**  
* **Editor interface**  
	The editor contains a Dock and a Toolbar.  
	Each of these can be detached and reattached to create different layouts.  
	The window size and layout are preserved across runs of the program.  
* **Settings**  
	There exists a page for settings accessed via the File menu or the keyboard shortcut "Ctrl+P"  
	Font size and color can be adjusted as well as autosave interval and maximum number of autosaves to store.  
	It is highly recommened that you store each project in its own unique folder as Chronicler creates subfolders for "scenes" and "backups" in the same directory as the .chronx project file. (Currently there is no warning for creating projects in the same folder as other projects)
* **Homepage**  
	The homepage displays an html news page and a list of recently opened projects on the left.  
	Below this are options to Create, Load, or Import projects.  
	These options are also available under the File menu.  
* **Project tab**  
	After opening a project, you are presented with a dock containing three tabs.  
	The project tab holds basic information about your project. Currently these options are the Title & Author of your game, and the scenes your project contains.  
	The scene list has buttons on the right for creating or deleting scenes and adjusting their order.  
	It is possible to undo if you accidently delete a scene.  
	The "startup" scene cannot be renamed or moved in the order. (ChoiceScript limitation)  
* **Variables tab**  
	This tab allows you to create and edit choicescript variables.  
	The first cell allows you to choose whether the variable is global to the entire game, or local to a specific scene.  
	The second cell is the name of the variable.  
	The third cell is optional and allows you to initialize the variable with a specific value.  
	(i.e. | Global | player_skill | 23 |)  
* **Bubble tab**  
	This tab in the dock displays editable information about the currently selected bubble.  
	All bubble controls contain a Label and a lockable Order.  
	The label is used when necessary for the generated ChoiceScript.  
	Order is the order in which this bubble is processed by the ChoiceScript compiler.  
	(Currently this is just a suggestion to the compiler, and may have no effect on the generated CS code) 	
* **Scene view**  
	Upon selecting a scene from the project tab, it opens as a tab in the main window.  
	Every scene contains a default start bubble which cannot be deleted.  
	The toolbar contains icons for each placeable bubble type.  
	Hovering over each icon displays a simple tooltip and, in some cases, usage instructions.  
	Holding down the Shift key will allow you to place multiple bubbles or connections.  
	When in Cursor mode, holding down shift and dragging from a blank area of the scene will allow you to select multiple bubbles.  
	You can also hold Ctrl to individually select/deselect multiple bubbles.  
* **Link tool**  
	The link tool allows you to create links between bubbles.  
	Left click and drag between bubbles to create a directional link between them.  
	Right click on a bubble to delete a link.  
	Links are attached to anchors on a bubble. Dragging from the bottom of one bubble to the top of another, will attach the link to the nearest available anchors at those positions.  
	**In the case of Condition bubbles, the** ***left anchor is the "true" output***, **and the** ***right anchor is the "false" output***.  
* **Palette tool**  
	The palette tool allows you to create and apply palettes to bubbles.  
	Right click on the tool itself to add a new palette.  
	Right click on an existing palette to edit it.  
	By default the tool contains palettes for each type of bubble in the scene.  
	Palettes are shared between bubbles, so if you edit an existing palette, all bubbles with that palette will be updated.  
	(Currently there is no way to delete palettes)  


**Inspiration:**  
Two years ago I began work on a ChoiceScript adventure game and quickly became bogged down in all the branching paths. I thought to myself, "_Would it not be easier to view this in a flowchart?_" Thus [Tinker](https://forum.choiceofgames.com/t/tool-tinker-visual-code-editor/6207) was born, followed 6 months later by [Chronicler](https://forum.choiceofgames.com/t/tool-chronicler-choicescript-visual-code-editor/6811). Both versions were developed in Game Maker Studio which currently has poor cross-platform support. As such I have decided to switch to Qt for further development.
Sadly, I can no longer remember what the CS game I wanted to make was...

![](https://licensebuttons.net/l/by-nc/4.0/88x31.png)  
**Chronicler** is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/)

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
