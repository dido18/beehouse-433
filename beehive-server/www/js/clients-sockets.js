var socket = io.connect('http://127.0.0.1:8080', {//'//' + location.host, {
  'reconnect': true,
  'reconnection delay': 50,
  'max reconnection attempts': 300
});

// socket.on('beemsg', function(data){
//   console.log(data);
//
// });


// $('form').submit(function(e){
//      e.preventDefault();
//      var node = $('#node').val();
//      var op = $('#op').val();
//      var data = $('#data').val();
//
//      socket.emit('command', JSON.stringify({"dst": node, "op":op, "data":data}));
//      console.log("COMMAND sent: " + JSON.stringify({"dst": node, "op":op, "data":data}));
// });
//
// </script>
