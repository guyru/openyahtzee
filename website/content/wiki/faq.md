Title: FAQ
Slug: FAQ

I collected here some frequently asked questions. If you still got an
unanswered question, please
[ask](http://www.guyrutenberg.com/contact-me)

## Gameplay

### What is the Yahtzee Bonus and how do I get one?

Yahtzee Bonus is a special bonus worth 100 points given for scoring
multiple Yahtzees. In order to receive a Yahtzee Bonus, you should score
a Yahtzee in the Yahtzee score box. From now, each additional Yahtzee
you get, you will receive a Yahtzee Bonus upon scoring it in any of the
open boxes.

Note: You'll only see the bonus after you score it in any of the
available slots.

### How do I get the bonus in the Upper Section?

You get the bonus in the upper section if you score 63 points or more in
the Upper Section. The bonus is worth 35 points.

## Compiling

### What are the requirements to compile Open Yahtzee?

Dependencies for Open Yahtzee 1.9 are:

 * gcc (>= 4.6) 
 * wxWidgets >= 2.8
 * Boost >= 1.32

### I compiled Open Yahtzee against wxWidgets-2.6.4 and some parts of the game seems broken. Why?

wxWidgets-2.6.4 introduces couple of new bugs, which unfortunately break
some of the functionality (mainly the items in the About dialog and
changing the layout to horizontal). My Advice it to upgrade to
wxWidgets-2.8 branch, or downgrade back to wxWidgets-2.6.3 and recompile
the game.

### Compling in Windows

To compile Open Yahtzee in Windows, adjust the paths of `WXWIN` and
`BOOST_ROOT` in `PropertySheet.props` and use the provided Visual Studio
Solution file (`openyahtzee.sln`).

## Misc
### Where does Open Yahtzee stores it's configuration and data?
It depends on your operating system:

 * Windows XP: `C:\Documents and Settings\USERNAME\Application Data\.openyahtzee`
 * Windows Vista/7: `C:\Users\USERNAME\AppData\Roaming\.openyahtzee`
 * Linux: `~/.openyahtzee`

