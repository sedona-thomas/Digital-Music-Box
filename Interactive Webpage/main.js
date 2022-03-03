/*
 * main.js: 
 * 
 * @author Sedona Thomas
 */

var x;
const button = document.querySelector('button');

function some_function() { }

button.addEventListener('click', function () {
    some_function();
    return;
}, false);