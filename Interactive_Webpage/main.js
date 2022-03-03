/*
 * main.js: 
 * 
 * @author Sedona Thomas
 */

var esp32BaudRate = 115200;
var singleInt = false; // if single integers are being sent over the data stream
var tags = false; // if HTML style tags are being sent over the data stream

// Document Click EventListener: triggers when user clicks anywhere on the page
document.addEventListener('click', async () => {
    setupPort();
    setupReader();
    readLoop();
});

// setupPort(): asks user to select port
async function setupPort() {
    var port = await navigator.serial.requestPort();
    await port.open({ baudRate: esp32BaudRate });
}

// setupReader(): sets up a serial port input stream reader
function setupReader() {
    let decoder = new TextDecoderStream();
    inputDone = port.readable.pipeTo(decoder.writable);
    inputStream = decoder.readable;
    var reader = inputStream.getReader();
}

// readLoop(): reads and processes input stream values
async function readLoop() {
    counterVal = 0;
    while (true) {
        const { value, done } = await reader.read();
        if (done) {
            // Allow the serial port to be closed later.
            console.log("closing connection")
            reader.releaseLock();
            break;
        }
        if (value) {
            data = parseValue(value);
            if (!isNaN(data)) {
                counterVal += data / 100.0;
                changeBackgroundColor(counterVal);
            }
        }
    }
};

// parseValue(): parses value of serial input stream
function parseValue(value) {
    if (singleInt) {
        return parseInt(value);
    } else if (tags) {
        return parseTags(value);
    } else {
        return parseJSON(value);
    }
}

// parseTags(): parses input as HTML style tags
function parseTags(value) {
    // needs to be written
    return;
}

// parseJSON(): parses input as JSON format
function parseJSON(value) {
    return JSON.parse(value)["data"];
}

// changeBackgroundColor(): updates the background color for current counter value
function changeBackgroundColor(counterVal) {
    redVal = (1 + Math.sin(counterVal)) * (255 / 2);
    greenVal = 60;
    blueVal = 50;
    document.body.style.backgroundColor = 'rgb(' + redVal + ',  ' + greenVal + ', ' + blueVal + ')';
}
