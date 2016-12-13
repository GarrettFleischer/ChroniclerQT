//#include "cchoicescriptmodel.h"

//#include "cgraphicsscene.h"


//CChoicescriptModel::CChoicescriptModel(CGraphicsScene *scene)
//    : m_scene(scene)
//{}


//QString CChoicescriptModel::BubbleToChoiceScript(const QList<CBubble *> &bubbles, QList<CBubble *> &processed, int indent_level, CBubble *bubble)
//{
//    QString cs;

//    if(!processed.contains(bubble))
//    {
//        processed.prepend(bubble);

//        QString indent_str = "    ";
//        QString indent;

//        // generate label...
//        if(LabelNeeded(bubble, bubbles))
//            cs +=  "\n\n*label " + MakeLabel(bubble, bubbles);
//        else
//            indent = QString(indent_str).repeated(indent_level);

//        // ------------ Start bubble ------------
//        if(bubble->getType() == Chronicler::StartBubble)
//        {
//            CStartBubble *start = static_cast<CStartBubble *>(bubble);
//            if(start->link())
//                cs += BubbleToChoiceScript(bubbles, processed, indent_level, start->link()->to());
//            else
//                cs += indent + "*finish\n";
//        }

//        // ------------ Story bubble ------------
//        if(bubble->getType() == Chronicler::StoryBubble)
//        {
//            CStoryBubble *story = static_cast<CStoryBubble *>(bubble);

//            cs += indent + story->getStory().replace("\n", "\n" + indent) + "\n";

//            if(story->link())
//            {
//                if(LabelNeeded(story->link()->to(), bubbles))
//                    cs += indent + "*goto " + MakeLabel(story->link()->to(), bubbles);
//                //                if(story->link()->to()->getType() == Chronicler::Story)
//                //                    cs += indent + "*page_break";
//                else
//                    cs += "\n" + BubbleToChoiceScript(bubbles, processed, indent_level, story->link()->to());
//            }
//            else
//                cs += indent + "*finish";
//        }

//        // ------------ Choice bubble ------------
//        else if(bubble->getType() == Chronicler::ChoiceBubble)
//        {
//            CChoiceBubble *choice_bubble = static_cast<CChoiceBubble *>(bubble);

//            cs += indent + "*choice";

//            // increase indent level
//            indent = indent_str.repeated(++indent_level);

//            for(CChoice *choice : choice_bubble->choiceBubbles())
//            {
//                QString hash = (choice->text().contains("#") ? "" : "#");
//                cs += "\n" + indent + hash + choice->text() + "\n";

//                if(choice->link() && LabelNeeded(choice->link()->to(), bubbles))
//                    cs += indent + indent_str + "*goto " + MakeLabel(choice->link()->to(), bubbles);
//                else if(choice->link())
//                    cs += "\n" + BubbleToChoiceScript(bubbles, processed, indent_level + 1, choice->link()->to());
//                else
//                    cs += indent + indent_str + "*finish";

//                cs += "\n";
//            }
//        }

//        // ------------ Action bubble ------------
//        else if(bubble->getType() == Chronicler::ActionBubble)
//        {
//            CActionBubble *action = static_cast<CActionBubble *>(bubble);

//            cs += indent + action->actionString().replace("\n", "\n" + indent) + "\n";

//            if(action->link() && LabelNeeded(action->link()->to(), bubbles))
//                cs += indent + "*goto " + MakeLabel(action->link()->to(), bubbles);
//            else if(action->link())
//                cs += "\n" + BubbleToChoiceScript(bubbles, processed, indent_level, action->link()->to());
//            else
//                cs += indent + "*finish";
//        }

//        // ------------ Condition bubble ------------
//        else if(bubble->getType() == Chronicler::ConditionBubble)
//        {
//            CConditionBubble *cb = static_cast<CConditionBubble *>(bubble);

//            cs += indent + "*if(" + cb->getCondition() + ")\n";

//            // true
//            if(cb->trueLink())
//            {
//                if(LabelNeeded(cb->trueLink()->to(), bubbles))
//                    cs += indent + indent_str + "*goto " + MakeLabel(cb->trueLink()->to(), bubbles);
//                else
//                    cs += BubbleToChoiceScript(bubbles, processed, indent_level + 1, cb->trueLink()->to());
//            }
//            else
//                cs += indent + indent_str + "*finish";

//            cs += "\n" + indent + "*else\n";

//            // false
//            if(cb->falseLink())
//            {
//                if(LabelNeeded(cb->falseLink()->to(), bubbles))
//                    cs += indent + indent_str + "*goto " + MakeLabel(cb->falseLink()->to(), bubbles);
//                else
//                    cs += BubbleToChoiceScript(bubbles, processed, indent_level + 1, cb->falseLink()->to());
//            }
//            else
//                cs += indent + indent_str + "*finish";
//        }

//        // ------------ Code bubble ------------
//        else if(bubble->getType() == Chronicler::CodeBubble)
//        {
//            CCodeBubble *code = static_cast<CCodeBubble *>(bubble);

//            cs += indent + code->getCode().replace("\n", "\n" + indent) + "\n";

//            if(code->link())
//            {
//                if(LabelNeeded(code->link()->to(), bubbles))
//                    cs += indent + "*goto " + MakeLabel(code->link()->to(), bubbles);
//                else
//                    cs += "\n" + BubbleToChoiceScript(bubbles, processed, indent_level, code->link()->to());
//            }
//        }
//    }

//    return cs;
//}
