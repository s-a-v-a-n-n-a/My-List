"# My-List"
h1 **DOCUMENTATION**
================
This is my realisation of List. 

h2 **DESCRIPTION OF STRUCT**
------------------------
List made on struct containing arrays with elements, indexes of next elemenet and previous elements.

h2 **FUNCTIONAL**
-------------
Its functional contains constructions, destructions, newing, deleting, inserting element before physical index,
removing element before physical index, inserting to the end and beginning of the list, removing from the end and beginning
of the list. There are functions to get physical index by logical one and sortion for quick access.


h2 **RECOMMANDATIONS**
------------------
TheThe functions of getting physical indexes and sortion are very slow, because they work for O(n), 
where n is the amount of elements in the list. Avoid using them by putting elements at the end of the list.

h2 **THE EXAMPLES OF WORK**
---------------------------
The examples of work of ***list_print_list_appearance***, which is visualisation of dump.

###h3 When list works coorectly, without invasions:
<li>
* Logical connections
![logical connection ok](https://github.com/s-a-v-a-n-n-a/My-List/blob/main/example_of_ok.txt.png)
* Physical connections
![logical connection ok](https://github.com/s-a-v-a-n-n-a/My-List/blob/main/example_real_of_ok.txt.png)

###h3 When list works uncoorectly, there were some invasions:
<li>
* Logical connections
![logical connection bad](https://raw.githubusercontent.com/s-a-v-a-n-n-a/My-List/main/example.txt.png)
* Physical connections 
![physical connection bad](https://github.com/s-a-v-a-n-n-a/My-List/blob/main/example_real.txt.png)
 
