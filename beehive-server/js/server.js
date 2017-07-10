var express = require('express'); // Get the module
var app = express();
var server = require('http').Server(app);
var bodyParser = require('body-parser');
var config = require('../config.json'); // config file
var io = require('socket.io')(server);


module.exports = function(_cb){

  // Setting up parameters passed by CLI
  //if (argv.username && argv.password) { config.username = argv.username.trim(); config.password = argv.password.trim(); }
  config.server_port = process.env.PORT || config.server_port;

  // Starting Server
  server.listen(config.server_port);
  console.info('Server started on', getLocalIPAddress(), '- Port', config.server_port);

  // set middleware
  app.use(bodyParser.json());

  // set .html as the default extension
  app.set('view engine', 'html');

  app.use(express.static('www'));

  app.set('views', __dirname + '/../www/views');



  //http.use(express.static(__dirname + '/node_modules'));

  // http.get('/', function(req, res,next) {
  //     res.sendFile('../www/index.html'); // __dirname +
  // });

  // API and Web Server + Socket part
  _cb({http: app, io: io});
};


// utility function
function getLocalIPAddress() {
  // synchronous method
  var interfaces = require('os').networkInterfaces();
  var IPs = [];
  for (var devName in interfaces) {
    var iface = interfaces[devName];

    for (var i = 0; i < iface.length; i++) {
      var alias = iface[i];
      if (alias.family === 'IPv4' && alias.address !== '127.0.0.1' && !alias.internal)
        IPs.push(alias.address);
    }
  }

  if (IPs.length === 1) return IPs[0];
  else if(IPs.length > 1) return IPs.toString();
  else return '0.0.0.0';
}
