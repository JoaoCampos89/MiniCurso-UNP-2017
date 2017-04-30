/*

// counter starts at 0
Session.setDefault('counter', 0);
// counter starts at 0
Session.setDefault('temperature', 0);
Template.hello.helpers({
  counter: function() {
    return Session.get('counter');
  },
  temperature: function() {
    return Session.get('temperature');
  }
});

Template.hello.events({
  'click button': function() {
    // increment the counter when button is clicked
    Session.set('counter', Session.get('counter') + 1);
  }
});
*/



Template.appPage.helpers({
  teams: function() {

    var teams = _.map(Teams, function(team) {
      var counter = 0;
      var result = Counter.findOne({
        _id: team
      });

      if (result) {
        counter = result.count;
      }

      return {
        name: team,
        counter: counter
      };
    });
    return teams;
  },

});
