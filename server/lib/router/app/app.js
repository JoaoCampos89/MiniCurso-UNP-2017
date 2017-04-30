// routes for app
//

var layoutPage = "layoutPage";

FlowRouter.route('/', {
  name: 'app',
  action: function() {
    return BlazeLayout.render(layoutPage, {
      main: 'appPage'
    });
  }
});

FlowRouter.route('/team/:team', {
  name: 'app',
  action: function() {
    return BlazeLayout.render(layoutPage, {
      main: 'teamPage'
    });
  }
});
