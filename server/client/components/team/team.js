Template.teamComponent.onCreated(function() {
  var template = this;
});



Template.teamComponent.helpers({
  name: function() {
    var template = Template.instance();
    return template.data.team.name;

  },
  totalMsg: function() {
    var template = Template.instance();
    var totalMsg = template.data.team.counter;
    return totalMsg;
  },
});

Template.teamComponent.events({
  "click .team": function(event, template) {

    FlowRouter.go("/team/" + template.data.team.name);
  }
});
