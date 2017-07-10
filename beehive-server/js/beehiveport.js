
var SerialPort = require('serialport');

var RadioHead433mhz = function( ){

	var port;

	function InitVariable() {
		choose_port();
	}
	InitVariable();



	_constructor();

	this.openSerialPort = function (onOpen){
    console.log(type);
		serial.open(function (error) {
		  if ( error ) {
		    debug('failed to open: '+error);
		    throw error;
		  } else {
		     debug('Serial port opened');
	             onOpen();
		  }
		});
	};

	this.on = function(callback){ // Receiving data from Arduino serial port
			serial.on('data', function(data){
			try{
						callback(JSON.parse(data)); // returning JSON parsed
			}catch(e){
						console.log('Error parsing this JSON: ', data);
			}
	});

	};

	this.sendMsg = function(msg, callback){
			if (serial.isOpen()){
				serial.write(String(msg), callback);
			}else{
				debug('SerialPort not open!');
			}
	};

	this.isSerialOpen = function(){
			return serial.isOpen();
	};

	this.close = function(callback){
			serial.close(callback); // release the Serial Port
	};

	this.choose_port = function(){
		SerialPort.list(function (err, ports) {
		      console.log('Selectable serial ports:');
		      var k = 1;
		      ports.forEach(function(port) {
		        console.log('('+k+') '+port.comName); k++;
		      });
		      prompt.start();

		      prompt.get({properties: {port: {required: true, type: 'number', conform: function(value){
		            if (value > 0 && value <= ports.length) return true;
		            else return false;
		        }}}}, function (err, result) {
		            if (err) return console.error(err);
		            choosen_port=  ports[result.port-1].comName;
		            console.log('Choosen port:', ports[result.port-1].comName);
		            //port =  choosen_port;
		            serial = new SerialPort(choosen_port, {
		                parser: SerialPort.parsers.readline('\n'),
		                baudRate: 2600,
		                autoOpen: false // autoOpen flag set to false, manually need to open the port
		            });

		          });

		});
	}

};
