// connecting mosquitto in our server
var mqttClient = mqtt.connect({
  host: 'localhost',
  port: 1883
});



/**
 *  Using test mosquitto
 */
//var mqttClient = mqtt.connect('mqtt://test.mosquitto.org');


/*mqttClient = mqtt.connect([{
  host: 'test.mosquitto.org',
  port: 1883
}]);
*/
MessageServer = new Mongo.Collection(null);



_.each(Teams, function(team) {
//  mqttClient.subscribe("minicurso/" + team + "/sensor/value");
  //  mqttClient.subscribe("arduino-day/team0");
//  mqttClient.subscribe(team + "/LED/ESTADO/GET");
  mqttClient.subscribe(team + "/LDR/ESTADO/GET");
});



/*Meteor.setInterval(function(argument) {
  mqttClient.publish('arduino-day/team0/sensor/value', '85.12312');
}, 5000);*/



var messagePub;
Meteor.publish('messages', function(argument) {
  var self = this;
  var counter = {};
  var handle = MessageServer.find({}).observeChanges({
    added: function(id, fields) {
      var date = new Date();
      if (!counter[fields.topic]) {
        counter[fields.topic] = 0;
        self.added('counter', fields.topic, {
          count: counter[fields.topic]
        });

      }
      counter[fields.topic] = counter[fields.topic] + 1;

      self.changed('counter', fields.topic, {
        count: counter[fields.topic]
      });
      self.added('messages', id, fields);
    }
  });

  self.ready();
  ready = true;
  self.onStop(function() {
    handle.stop();
  });
});
/**
 * [Apagar mensagens no servidor a cada 100 segundos]
 * @return {[type]} [description]
 */
mqttClient.on('message', Meteor.bindEnvironment(function(topic, message) {
  //console.log("message arrived");
  var value = Number(message.toString());
  topic = topic.split("/");
  if (!isNaN(value)) {

    var msg = {
      measurament: value,
      topic: topic[0],
      field: topic[1],
      ts: new Date(),
    };
    MessageServer.insert(msg);
  } else {
    console.log("Your message is in invalid format: " + topic[1]);
  }
  //self.changed('messages', 1, msg);

}));


Meteor.methods({
  teamConnectLed: Meteor.bindEnvironment(function(argument) {
    check(argument.teamName, String);
    check(argument.ledStatus, Boolean);
    var led = "";
    if (argument.ledStatus) {
      led = "1";
    } else {
      led = "0";
    }
    var msg = led;
    console.log("sending command");
    console.log(argument.teamName + '/LED/ESTADO/SET');
    /*mqttClient.publish('minicurso/command/' + argument.teamName, msg,
      function(error) {
        if (error) {
          console.log(error);
        }

      });*/

    mqttClient.publish(argument.teamName + '/LED/ESTADO/SET', msg,
        function(error) {
          if (error) {
            console.log(error);
          }

        });

    return led;

  })
});

/**
 * [Apagar mensagens no servidor a cada 100 segundos]
 * @return {[type]} [description]
 */
Meteor.setInterval(function() {
  MessageServer.remove({});
}, 100000);
