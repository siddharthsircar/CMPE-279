Answer 1. To do the SQL injection on DVWA, I utilized the text %' or '0'='0. This input string adds an always true condition and closes the first string parameter, which is meant to be the user Id. Now that the query has been processed as True, every user in the database will be returned.

Answer 2. No, it won't work because when we set the security to medium, there is a dropdown list from which we must choose the id from the pre-provided list rather than a textbox to add our input. Therefore, by eliminating the textbox entry and inserting the dropdown choices, they have addressed this problem.

Answer 3. <script>alert("You're hacked")</script> is the input I used for the XSS attack. The webpage incorrectly escapes the input string, thus this works. It accepts any user input and inserts the outcome into the DOM. The malicious Javascript I just supplied will then be executed and the alert of our choice will appear when the website is updated with user input.

Answer 4. This trick doesn't work on Medium difficulty possibly because the website now handles the input string correctly.
