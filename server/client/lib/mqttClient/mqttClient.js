Messages = new Mongo.Collection("messages");
Counter = new Mongo.Collection("counter");

/*Messages.find({}).observe({
  added: function(item) {
    if (item.topic === "temperature") {
      Session.set('temperature', item.message);
    }
  }
});*/
Tracker.autorun(function() {
  Meteor.subscribe("messages");
});
