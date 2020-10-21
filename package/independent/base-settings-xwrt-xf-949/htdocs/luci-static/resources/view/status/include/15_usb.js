'use strict';
'require baseclass';
'require fs';
'require rpc';

var callUsbInfo = rpc.declare({
	object: 'luci.usb',
	method: 'info'
});

return baseclass.extend({
	title: _('USB'),

	load: function() {
		return Promise.all([
			L.resolveDefault(callUsbInfo(), {})
		]);
	},

	render: function(data) {
		var usbinfo   = data[0];

		var fields = [
			_('Info'),         usbinfo.info ? usbinfo.info : "-"
		];

		var table = E('div', { 'class': 'table' });

		for (var i = 0; i < fields.length; i += 2) {
			table.appendChild(E('div', { 'class': 'tr' }, [
				E('div', { 'class': 'td left', 'width': '25%' }, [ fields[i] ]),
				E('div', { 'class': 'td left' }, [ (fields[i + 1] != null) ? fields[i + 1] : '?' ])
			]));
		}

		return table;
	}
});
