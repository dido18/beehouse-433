// app.js
var prompt = require('prompt');
var SerialPort = require('serialport');

var async = require('async');

var serial;
var serial = new SerialPort("/dev/ttyUSB0", {
    parser: SerialPort.parsers.readline('\n'),
    baudRate: 9600,
    autoOpen: false});
async.series({
    //   get_serial: function(callback){
    //     SerialPort.list(function (err, ports) {
    //           console.log('Selectable serial ports:');
    //           var k = 1;
    //           ports.forEach(function(port) {
    //             console.log('('+k+') '+port.comName); k++;
    //           });
    //           prompt.start();
     //
    //           prompt.get({properties: {port: {required: true, type: 'number', conform: function(value){
    //                 if (value > 0 && value <= ports.length) return true;
    //                 else return false;
    //             }}}}, function (err, result) {
    //                 if (err) return; //cb(null);
    //                 choosen_port=  ports[result.port-1].comName;
    //                 console.log('Choosen port:', ports[result.port-1].comName);
    //                 //port =  choosen_port;
    //                 serial = new SerialPort(choosen_port, {
    //                     parser: SerialPort.parsers.readline('\n'),
    //                     baudRate: 9600,
    //                     autoOpen: false // autoOpen flag set to false, manually need to open the port
    //                 });
    //                 callback(null, 1);
    //               });
    //     });
    //    //callback(null, 1);
    //  },
     init_rf: function(callback) {
            // Listen on Arduino Serial Port or RF433Mhz chip if on RPi platform.
            serial.open(function (error) {
      		  if ( error ) {
      		    console.log('failed to open: '+error);
      		    throw error;
      		  } else {
              setTimeout(function() {
                  callback(null, 1);
                  console.log('Open serial succesfully');
              }, 2000);
            }
      		    //callback(null, 1);
      		  });
            // serial.openSerialPort(function() {
            //     setTimeout(function() {
            //         callback(null, 1);
            //     }, 2000); // Arduino AutoReset requires to wait a few seconds before sending data!
            // });

        },
    server: function(callback) {
      require(__dirname +"/js/server")(function(server){
        var http = server.http;
        var io = server.io;   // socket.io

        require(__dirname + "/js/api")(http, io); // add the api to the server

        var socketFunctions = require('./js/sockets.js')(io, serial);//, rf433mhz, dbFunctions);


        /* LISTENERS of socket.io from htttp clients*/
       io.on('connection', socketFunctions.onConnection);

       // Listeners on socket communication from Beehive (Arduino Uno) through serial port
       serial.on('data', function(data){
         try{
           console.log( JSON.parse(data)); // returning JSON parsed
           io.emit('beehive', data);
         }catch(e){
           console.log('Error parsing this JSON: ', data);
         }
       });


      });
    },
    },  function(err, results) {

});
