/*
 * main.js: 
 * 
 * @author Sedona Thomas
 */

var esp32BaudRate = 115200;
var singleInt = false; // if single integers are being sent over the data stream
var tags = false; // if HTML style tags are being sent over the data stream

var port;
var reader;

var audioCtx;
var activeOscillators = {};
var activeGainNodes = {};
var mode = 'single';
var waveform = 'sine';
var lfo = false;
var numberOfPartials = 5;
var partialDistance = 15;
var modulatorFrequencyValue = 100;
var modulationIndexValue = 100;
var lfoFreq = 2;
var octave = 1;
var octaves = [1 / 4, 1 / 2, 1, 2, 4, 8];

const keyboardFrequencyMap =
{
    '90': 261.625565300598634,  //Z - C
    '83': 277.182630976872096,  //S - C#
    '88': 293.664767917407560,  //X - D
    '68': 311.126983722080910,  //D - D#
    '67': 329.627556912869929,  //C - E
    '86': 349.228231433003884,  //V - F
    '71': 369.994422711634398,  //G - F#
    '66': 391.995435981749294,  //B - G
    '72': 415.304697579945138,  //H - G#
    '78': 440.000000000000000,  //N - A
    '74': 466.163761518089916,  //J - A#
    '77': 493.883301256124111,  //M - B
    '81': 523.251130601197269,  //Q - C
    '50': 554.365261953744192,  //2 - C#
    '87': 587.329535834815120,  //W - D
    '51': 622.253967444161821,  //3 - D#
    '69': 659.255113825739859,  //E - E
    '82': 698.456462866007768,  //R - F
    '53': 739.988845423268797,  //5 - F#
    '84': 783.990871963498588,  //T - G
    '54': 830.609395159890277,  //6 - G#
    '89': 880.000000000000000,  //Y - A
    '55': 932.327523036179832,  //7 - A#
    '85': 987.766602512248223,  //U - B
}

// for each note, the octave up is a less gray version of the color
const frequencyColorMap =
{
    '90': '#444444',  //Z - C
    '83': '#444488',  //S - C#
    '88': '#4444CC',  //X - D
    '68': '#4488CC',  //D - D#
    '67': '#44CC44',  //C - E
    '86': '#44CCCC',  //V - F
    '71': '#88CCCC',  //G - F#
    '66': '#CC4444',  //B - G
    '72': '#CC4488',  //H - G#
    '78': '#CC44CC',  //N - A
    '74': '#CC88CC',  //J - A#
    '77': '#CCCCCC',  //M - B
    '81': '#000000',  //Q - C
    '50': '#000088',  //2 - C#
    '87': '#0000FF',  //W - D
    '51': '#0088FF',  //3 - D#
    '69': '#00FF00',  //E - E
    '82': '#00FFFF',  //R - F
    '53': '#88FFFF',  //5 - F#
    '84': '#FF0000',  //T - G
    '54': '#FF0088',  //6 - G#
    '89': '#FF00FF',  //Y - A
    '55': '#FF88FF',  //7 - A#
    '85': '#FFFFFF',  //U - B
}

// Document Click EventListener: triggers when user clicks anywhere on the page
$("#start_button").on('click', async () => {
    removePrompt();
    await setupPort();
    setupReader();
    setupAudio();
    await readLoop();
});

window.addEventListener('keydown', keyDown, false);
window.addEventListener('keyup', keyUp, false);

// removePrompt(): removes starting prompt
function removePrompt() {
    let div = document.getElementById('start_prompt');
    div.parentNode.removeChild(div);
}

// setupPort(): asks user to select port
async function setupPort() {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: esp32BaudRate });
}

// setupReader(): sets up a serial port input stream reader
function setupReader() {
    let decoder = new TextDecoderStream();
    inputDone = port.readable.pipeTo(decoder.writable);
    inputStream = decoder.readable;
    reader = inputStream.getReader();
}

// setupAudio(): sets up audio context and initializes modes
function setupAudio() {
    audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    resetModes();
}

// reseteModes(): sets the initial values for the various modes
function resetModes() {
    mode = 'single';
    waveform = 'sine';
    lfo = false;
    numberOfPartials = 5;
    partialDistance = 15;
    modulatorFrequencyValue = 100;
    modulationIndexValue = 100;
    lfoFreq = 2;
}

// readLoop(): reads and processes input stream values
async function readLoop() {
    while (true) {
        const { value, done } = await reader.read();
        string = value;
        if (string.charAt(0) == "{") {
            while (!isJSON(string) && string.length < 1000) {
                const { value, done } = await reader.read();
                string += value;
            }
        }

        if (isJSON(string)) {
            if (done) {
                // Allow the serial port to be closed later.
                console.log("closing connection")
                reader.releaseLock();
                break;
            }
            if (string) {
                data = parseValue(string);
                controlKeyboardParameters(data);
            }
        }
    }
};

// controlKeyboardParameters(): manages sensor input to control keyboard
function controlKeyboardParameters(data) {
    octave = octaves[Math.floor(data["potentiometer_octave"] / (256 / octaves.length))];

    lfo = data["button_mode"] ? lfo : !lfo;

    lfoFreq = data["joystick_joystick1"]["potentiometer_x"] < 5 ? lfoFreq + 1 : lfoFreq;
    lfoFreq = data["joystick_joystick1"]["potentiometer_x"] > 250 && lfoFreq > 0 ? lfoFreq - 1 : lfoFreq;

    numberOfPartials = data["joystick_joystick1"]["potentiometer_y"] > 250 && numberOfPartials < 7 ? numberOfPartials + 1 : numberOfPartials;
    numberOfPartials = data["joystick_joystick1"]["potentiometer_y"] < 5 && numberOfPartials > 1 ? numberOfPartials - 1 : numberOfPartials;

    mode = data["joystick_joystick1"]["button_sw"] == 0 ? 'single' : mode;
    waveform = data["joystick_joystick1"]["button_sw"] == 0 ? 'sine' : waveform;
    lfo = data["joystick_joystick1"]["button_sw"] == 0 ? false : lfo;
    numberOfPartials = data["joystick_joystick1"]["button_sw"] == 0 ? 5 : numberOfPartials;
    partialDistance = data["joystick_joystick1"]["button_sw"] == 0 ? 15 : partialDistance;
    modulatorFrequencyValue = data["joystick_joystick1"]["button_sw"] == 0 ? 100 : modulatorFrequencyValue;
    modulationIndexValue = data["joystick_joystick1"]["button_sw"] == 0 ? 100 : modulationIndexValue;
    lfoFreq = data["joystick_joystick1"]["button_sw"] == 0 ? 2 : lfoFreq;

}

// parseValue(): parses value of serial input stream as JSON format
function parseValue(value) {
    if (isJSON(value)) {
        return JSON.parse(value)["data"];
    } else {
        return {}
    }
}

// isJSON(): checks if string is in appropriate JSON format
function isJSON(string) {
    try {
        JSON.parse(string);
    } catch (exception) {
        return false;
    }
    return true;
}

// changeBackgroundColor(): updates the background color for current counter value
function changeBackgroundColor(counterVal) {
    redVal = (1 + Math.sin(counterVal)) * (255 / 2);
    greenVal = 60;
    blueVal = 50;
    document.body.style.backgroundColor = 'rgb(' + redVal + ',  ' + greenVal + ', ' + blueVal + ')';
}

function updatePartialNum(value) { numberOfPartials = value; };
function updatePartialDistance(value) { partialSize = value; };
function updateFreq(value) { modulatorFrequencyValue = value; };
function updateIndex(value) { modulationIndexValue = value; };
function updateLfo(value) { lfoFreq = value; };

// buttons to switch modes
const singleButton = document.getElementById("single");
singleButton.addEventListener('click', function () { mode = 'single'; }, false);
const additiveButton = document.getElementById("additive");
additiveButton.addEventListener('click', function () { mode = 'additive'; }, false);
const AMButton = document.getElementById("am");
AMButton.addEventListener('click', function () { mode = 'am'; }, false);
const FMButton = document.getElementById("fm");
FMButton.addEventListener('click', function () { mode = 'fm'; }, false);

// buttons to switch between each waveform
const sineButton = document.getElementById("sine");
sineButton.addEventListener('click', function () { waveform = 'sine'; }, false);
const sawtoothButton = document.getElementById("sawtooth");
sawtoothButton.addEventListener('click', function () { waveform = 'sawtooth'; }, false);
const squareButton = document.getElementById("square");
squareButton.addEventListener('click', function () { waveform = 'square'; }, false);
const triangleButton = document.getElementById("triangle");
triangleButton.addEventListener('click', function () { waveform = 'triangle'; }, false);

// buttons to turn on and off lfo
const lfoOnButton = document.getElementById("lfoOn");
lfoOnButton.addEventListener('click', function () { lfo = true; }, false);
const lfoOffButton = document.getElementById("lfoOff");
lfoOffButton.addEventListener('click', function () { lfo = false; }, false);

// keyDown(): plays note of pressed key if not currently playing
function keyDown(event) {

    console.log(numberOfPartials);

    const key = (event.detail || event.which).toString();
    if (keyboardFrequencyMap[key] && !activeOscillators[key]) {
        if (mode == "single") {
            playNote(key);
        }
        else if (mode == "additive") {
            playNoteAdditive(key);
        }
        else if (mode == "am") {
            playNoteAM(key);
        }
        else if (mode == "fm") {
            playNoteFM(key);
        }

        // change background color to most recent note
        document.body.style.background = frequencyColorMap[key];
    }
}

// keyUp(): stops note of pressed key when key is released
function keyUp(event) {
    const key = (event.detail || event.which).toString();
    if (keyboardFrequencyMap[key] && activeOscillators[key]) {
        stopNote(key);
    }
}

// playNote(): plays the note for the current keyboard key
function playNote(key) {
    // create gain node and initialize as 0
    const gainNode = audioCtx.createGain();
    gainNode.gain.setValueAtTime(0, audioCtx.currentTime);

    // create oscillator and connect to gain node
    const osc = audioCtx.createOscillator();
    osc.frequency.setValueAtTime((keyboardFrequencyMap[key] * octave), audioCtx.currentTime);
    osc.type = waveform;
    osc.connect(gainNode).connect(audioCtx.destination);
    osc.start();

    // saves current gain node and oscillator
    activeGainNodes[key] = [gainNode];
    activeOscillators[key] = [osc];

    handleLFO(key, 10, osc);

    // attack (keeps total of gain nodes less than 1)
    let gainNodes = Object.keys(activeGainNodes).length;
    gainNode.gain.setTargetAtTime(0.7 / gainNodes, audioCtx.currentTime, 0.1);

    // decay then sustain
    Object.keys(activeGainNodes).forEach(function (gainNodeKey) {
        activeGainNodes[gainNodeKey][0].gain.setTargetAtTime(0.4 / gainNodes, audioCtx.currentTime, 0.1);
    });
}

// playNoteAdditive(): plays the note for the current keyboard key with additive synthesis
function playNoteAdditive(key) {
    // saves current gain node and oscillator
    activeGainNodes[key] = [];
    activeOscillators[key] = [];

    for (let i = 0; i < numberOfPartials; i++) {
        // create gain node and initialize as 0
        const gainNode = audioCtx.createGain();
        gainNode.gain.setValueAtTime(0, audioCtx.currentTime);

        // create oscillator and connect to gain node
        const osc = audioCtx.createOscillator();
        let freq = (keyboardFrequencyMap[key] * octave) * (i + 1);
        freq += ((i % 2) * -1) * (i + 1) * partialDistance * Math.random();
        osc.frequency.setValueAtTime(freq, audioCtx.currentTime);
        osc.type = waveform;
        osc.connect(gainNode).connect(audioCtx.destination);
        osc.start();
        activeGainNodes[key].push(gainNode);
        activeOscillators[key].push(osc);

        handleLFO(key, 10, osc);
    }

    // attack (keeps total of gain nodes less than 1)
    let gainNodes = Object.keys(activeGainNodes).length * numberOfPartials;
    for (let i = 0; i < activeGainNodes[key].length; i++) {
        activeGainNodes[key][i].gain.setTargetAtTime(0.7 / gainNodes, audioCtx.currentTime, 0.1);
    }

    // decay then sustain
    Object.keys(activeGainNodes).forEach(function (gainNodeKey) {
        for (let i = 0; i < activeGainNodes[key].length; i++) {
            activeGainNodes[gainNodeKey][i].gain.setTargetAtTime(0.4 / gainNodes, audioCtx.currentTime, 0.1);
        }
    });
}

// playNoteAM(): plays the note for the current keyboard key with AM synthesis
function playNoteAM(key) {
    let carrier = audioCtx.createOscillator();
    let modulatorFreq = audioCtx.createOscillator();
    carrier.type = waveform;
    carrier.frequency.setValueAtTime((keyboardFrequencyMap[key] * octave), audioCtx.currentTime);
    modulatorFreq.frequency.value = modulatorFrequencyValue;

    const modulated = audioCtx.createGain();
    const depth = audioCtx.createGain();
    depth.gain.value = 0.5; //scale modulator output to [-0.5, 0.5]
    modulated.gain.value = 1.0 - depth.gain.value; //a fixed value of 0.5

    // create gain node and initialize as 0
    const gainNode = audioCtx.createGain();
    gainNode.gain.setValueAtTime(0, audioCtx.currentTime);

    modulatorFreq.connect(depth).connect(modulated.gain);
    carrier.connect(modulated);
    modulated.connect(gainNode).connect(audioCtx.destination);

    carrier.start();
    modulatorFreq.start();

    // saves current gain node and oscillator
    activeGainNodes[key] = [gainNode, modulated, depth];
    activeOscillators[key] = [carrier, modulatorFreq];

    handleLFO(key, 300, modulatorFreq);

    // attack (keeps total of gain nodes less than 1)
    let gainNodes = Object.keys(activeGainNodes).length;
    gainNode.gain.setTargetAtTime(0.7 / gainNodes, audioCtx.currentTime, 0.1);

    // decay then sustain
    Object.keys(activeGainNodes).forEach(function (gainNodeKey) {
        activeGainNodes[gainNodeKey][0].gain.setTargetAtTime(0.4 / gainNodes, audioCtx.currentTime, 0.1);
    });
}

// playNoteFM(): plays the note for the current keyboard key with FM synthesis
function playNoteFM(key) {
    let modulatorFreq = audioCtx.createOscillator();
    modulatorFreq.frequency.value = modulatorFrequencyValue;

    let modulationIndex = audioCtx.createGain();
    modulationIndex.gain.value = modulationIndexValue;

    let carrier = audioCtx.createOscillator();
    carrier.type = waveform;
    carrier.frequency.value = (keyboardFrequencyMap[key] * octave);

    modulatorFreq.connect(modulationIndex);
    modulationIndex.connect(carrier.frequency);

    // create gain node and initialize as 0
    const gainNode = audioCtx.createGain();
    gainNode.gain.setValueAtTime(0, audioCtx.currentTime);
    carrier.connect(gainNode).connect(audioCtx.destination);

    carrier.start();
    modulatorFreq.start();

    // saves current gain node and oscillator
    activeGainNodes[key] = [gainNode, modulationIndex];
    activeOscillators[key] = [carrier, modulatorFreq];

    handleLFO(key, 300, modulatorFreq);

    // attack (keeps total of gain nodes less than 1)
    let gainNodes = Object.keys(activeGainNodes).length;
    gainNode.gain.setTargetAtTime(0.7 / gainNodes, audioCtx.currentTime, 0.1);

    // decay then sustain
    Object.keys(activeGainNodes).forEach(function (gainNodeKey) {
        activeGainNodes[gainNodeKey][0].gain.setTargetAtTime(0.4 / gainNodes, audioCtx.currentTime, 0.1);
    });

}

// stopNote(): stops the note for the current keyboard key
function stopNote(key) {
    // release
    for (let i = 0; i < activeGainNodes[key].length; i++) {
        activeGainNodes[key][i].gain.cancelScheduledValues(audioCtx.currentTime);
        activeGainNodes[key][i].gain.setTargetAtTime(0, audioCtx.currentTime, 0.01);
        delete activeGainNodes[key][i];
    }

    for (let i = 0; i < activeOscillators[key].length; i++) {
        activeOscillators[key][i].stop(audioCtx.currentTime + 0.05);
        delete activeOscillators[key][i];
    }

    // deletes current gain node and oscillator
    delete activeGainNodes[key];
    delete activeOscillators[key];
}

function handleLFO(key, value, osc) {
    if (lfo) {
        let lfo = audioCtx.createOscillator();
        lfo.frequency.value = lfoFreq;
        let lfoGain = audioCtx.createGain();
        lfoGain.gain.value = value;
        lfo.connect(lfoGain).connect(osc.frequency);
        lfo.start();
        activeOscillators[key].push(lfo);
    }
}