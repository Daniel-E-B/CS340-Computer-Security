# ui validation
### break something and fix it

## c
### buffer overflow
Getting it to overflow was very easy. I compiled it using the given flags and read the c file and saw this `char password_buffer[16];`. Obviously the input buffer has a length of 16. Interestingly, I found that to break the program, I needed to use >20 characters, instead of >16. I think that this is because the buffer overflow overwrites `auth_flag` to be a value greater than 1 (triggering the if statement in main to be true), and auth_flag is not stored immediately next to `password_buffer`, so some extra memory needs to be overflowed before `auth_flag` is overwritten. The input to the function can also be overwritten if enough characters are typed, which causes the program to seg fault.
#### patching
Patching the buffer overflow issue was also fairly straightforward. It involved using `strncpy()` instead of `strcpy()`, the difference being that the latter has a third argument for the maximum number of characters to be copied from the source. By setting this to the `sizeof` the buffer c string, the program can be overflow-proofed:
```c
strcpy(password_buffer, password);
```
was changed to
```c
strncpy(password_buffer, password, sizeof(password_buffer));
```
### breaking with lldb (mac gdb)
I wanted to try something more challenging, so I looked at the course resources and found this video:
<a href="http://www.youtube.com/watch?feature=player_embedded&v=VroEiMOJPm8
" target="_blank"><img src="http://img.youtube.com/vi/VroEiMOJPm8/0.jpg" 
 width="1280" height="720" border="0" /></a>
I tried the techniques used in the video, and found two different ways of cracking the program by setting the eax register before a compare instruction. My notes for going through the assembly can be found in `c/authentication think through of dump.rtf` and `c/main think through of dump.rtf`. An rtf viewer is recommended, because without being able to see what is bolded it will be illegible.
  * In my opinion, the best way to create an authentication program that cannot be broken using this method is to do the actual authentication on a remote server, so that any user with malicious intent cannot acces the program that is running the auth checks. Of course things brings in network and encryption security concerns, but it does eliminate the issue that was exploited here.

## python
### injecting code
```python
favorite = input('What is your favorite number? ')
print 'I like the number {}, too!'.format(favorite)
```
When given the input prompt `What is your favorite number?`, I inputted ` __import__('os').system('ls /Applications’)`. I learned about `__import__` from searching "python import without spaces".

After some more searching and playing around, I realized that using a [ ], ( ), or just commas, I  could run more than one line of code. 

![alt text](https://github.com/Daniel-E-B/CS340-Computer-Security/raw/master/uiValidation/python/screenshot.png )

I found it interesting that if I was using an imported library like in the first and second examples, I needed to import it every time.

### patching
Patching this issue is very simple. There are two main ways of doing it.
1. Use python3. the python3 `input()` function does not do an `eval()` on the input, and is therefore invulnerable to this issue.
2. If somehow you still want to use python2, use `raw_input()` because it, too does not do an `eval()`.

## web
### injecting javascript
I wanted a way of running code without the user having to do anything other than add the comment. This pretty much left me with the onload DOM event, which I used with an image of Rick Astley.
```html
<img src = "https://i.ytimg.com/vi/dQw4w9WgXcQ/maxresdefault.jpg"
onload="
setTimeout(hax, 100);
function hax(){
window.alert('never gonna give you up');
document.getElementsByTagName('body')[0].innerHTML='prankd';}">
```
### patching
Patching the javascript was fairly simple. In my struggle to learn markdown to write this writeup, I learned about HTML escape characters. To patch the comments program, I used the javascript string `.replace()` method to replace `<` and `>`with their escape characters. I only had to modify one line:
```javascript
textDiv.innerHTML = document.getElementById('commentText').value;
```
was changed to
```javascript
textDiv.innerHTML = document.getElementById('commentText').value.replace(">","&gt;").replace("<","&lt;");
```

Now when I input my string of injection code, it just pops up in the comments list like any other comment!

## conclusion
All of these exploits have potentially very dangerous repercussions, but very easy and simple fixes.

