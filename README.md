# ![](https://forum.choiceofgames.com/uploads/choiceofgames/270/107efc2878dfc5fc.png) Chronicler  
**Description:**  
Chronicler is a cross-platform visual code editor for [ChoiceScript](https://www.choiceofgames.com/make-your-own-games/choicescript-intro/). It allows both beginners and veteran novelists alike to design interactive fiction with a flowchart-like layout rather than typing code in a text editor.

**Technologies:**  
C++, Qt, QML, Javascript

**Screenshots:**  
[Choice of the Dragon](https://www.choiceofgames.com/dragon/) example Chronicler project.  
![](https://dl.dropboxusercontent.com/s/y6ygh2ma8bkyjwq/Chronicler1.png?dl=0)  
![](https://dl.dropboxusercontent.com/s/12unpg8wvym6bix/Chronicler7.png?dl=0)  
![](https://dl.dropboxusercontent.com/s/s4qjfcttlua7tu1/Chronicler5.png?dl=0)  

**Installation:**  
1. Install Qt Creator (5.5)  
2. Import ChroniclerQT project  
3. Build->Release  

**Contribution Guidelines:**  
Naming convention: Prefix all .h .cpp and .qml files with "C"

**TODO:**  
* Implement all existing features of GM:S version in Qt
 * Editable Story/Condition/Choice bubbles
 * Links between bubbles
 * .chron file import
 * CS code generation
 * CS code import

**Bugs:**  
* Mouse cursor in improper state when adding connections to the scene
* Font changes only apply to existing objects

**Contributors:**  
[Garrett Fleischer](https://www.linkedin.com/in/garrett-fleischer-57a230b7)


**Inspiration:**  
Two years ago I began work on a ChoiceScript adventure game and quickly became bogged down in all the branching paths. I thought to myself, "_Would it not be easier to view this in a flowchart?_" Thus [Tinker](https://forum.choiceofgames.com/t/tool-tinker-visual-code-editor/6207) was born, followed 6 months later by [Chronicler](https://forum.choiceofgames.com/t/tool-chronicler-choicescript-visual-code-editor/6811). Both versions were developed in Game Maker Studio which currently has poor cross-platform support. As such I have decided to switch to Qt for further development.

![](https://licensebuttons.net/l/by-nc/4.0/88x31.png)  
**Chronicler** is licensed under a [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/)

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.