# CMPE 279 Assignment 4

## Questions
I chose to do the stored XSS attack
1. The attack I used is enter this string :
```
<a href="https://www.google.com">Click ME</a>
```
This will appear in the message as a clickable link. This is only to show how the website will display whatever message a user submits.

![image](https://user-images.githubusercontent.com/24874394/205553450-d5599218-180b-455e-a3f1-22151c65608f.png)

2. On **Medium**, the trick above is not working anymore. The website removed all html tags and only preserved string.

![image](https://user-images.githubusercontent.com/24874394/205553536-5c012b39-6aff-4f19-800b-19cfba5342d4.png)

3. On **Low**, the website just removed slashes from the _message_ and did not sanitize the name's input. Because of this, it is open to all forms of HTML/JavaScript assaults. On **High**, they use more sophisticated techniques to sanitize both the _message_ and the _name_. This includes a regex that replaces everything that will result in a "script", strip tags, and htmlspecialchars.
