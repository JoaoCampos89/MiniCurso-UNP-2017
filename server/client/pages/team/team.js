Template.teamPage.onCreated(function() {
  var template = this;
  template.displayChart = new ReactiveVar(true);
  template.connectLed = new ReactiveVar(false);
  var urlParam = FlowRouter.getParam("team");
  var messages = Messages.find({
    topic: urlParam
  }).fetch();
  template.msgArray = messages;
});



Template.teamPage.helpers({
  name: function() {
    return FlowRouter.getParam("team");

  },
  settings: function() {
    var template = Template.instance();
    var urlParam = FlowRouter.getParam("team");
    var messages = Messages.find({
      topic: urlParam
    }).fetch();


    return {
      collection: messages,
      rowsPerPage: 10,
      showFilter: true,
      fields: ['ts', 'measurament']
    };
  },
  textButton: function() {
    var template = Template.instance();
    if (template.displayChart.get()) {
      return "Tabela";
    }
    return "Chart";
  },
  connectLed: function() {
    var template = Template.instance();
    if (template.connectLed.get()) {
      return "Desliga Led";
    } else {
      return "Liga Led";
    }
  },
  btnConnectLed: function() {
    var template = Template.instance();
    if (template.connectLed.get()) {
      return "btn-danger";
    } else {
      return "btn-info";
    }
  },
  displayChart: function() {
    var template = Template.instance();
    return template.displayChart.get();
  },
  data: function() {
    var urlParam = FlowRouter.getParam("team");
    var messages = Messages.find({
      topic: urlParam
    }, {
      limit: 500,
      sort: {
        ts: -1
      }

    }).fetch();

    return messages;
  }


});

Template.teamPage.onRendered(function() {
  var template = this;

});

Template.teamPage.events({
  'click a.btn.btn-default.ver-chart': function(event, template) {
    template.displayChart.toggle();
  },
  'click .ligar-led': function(event, template) {
    Meteor.call("teamConnectLed", {
      ledStatus: template.connectLed.get(),
      teamName: FlowRouter.getParam("team")
    }, function(error, result) {
      if (error) {
        console.log("error", error);
      }
      if (result) {
        template.connectLed.toggle();
      }
    });
  },
  'click button.voltar': function(event, template) {

    FlowRouter.go("/");
  },

});
