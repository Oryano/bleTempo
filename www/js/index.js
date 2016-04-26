
/*
  BLE Central

  This example uses Don Coleman's BLE Central Plugin for Apache Cordova
  to create a central server that connects and sends data to the Light Blue Bean.

  created 29 Mar 2015
  by Maria Paula Saba
*/



/* global mainPage, deviceList, refreshButton */
/* global connectedPage, resultDiv, messageInput, sendButton, disconnectButton */
/* global ble  */
/* jshint browser: true , devel: true*/
'use strict';

var DEVICE = "Tempo";
var scratchServiceUUID = 'A495FF20-C5B1-4B44-B512-1370F02D74DE';
var scratchCharacteristicUUID = 'A495FF21-C5B1-4B44-B512-1370F02D74DE';


/////////////////for bootstrap slider
// var slider = new Slider("#ex13", {
//     ticks: [1, 5, 15, 30, 60],
//     ticks_labels: ['1', '5', '15', '30', '60'],
//     ticks_snap_bounds: 30
// });


function toggleButton(){
	////jquery for toggle button
	$('.btn-toggle').click(function() {
		console.log('btn-toggle clicked')
			$(this).find('.btn').toggleClass('active');

			if ($(this).find('.btn-primary').size()>0) {
				$(this).find('.btn').toggleClass('btn-primary');
			}
			if ($(this).find('.btn-danger').size()>0) {
				$(this).find('.btn').toggleClass('btn-danger');
			}
			if ($(this).find('.btn-success').size()>0) {
				$(this).find('.btn').toggleClass('btn-success');
			}
			if ($(this).find('.btn-info').size()>0) {
				$(this).find('.btn').toggleClass('btn-info');
			}

			$(this).find('.btn').toggleClass('btn-default');

	});

	$('form').submit(function(){
		alert($(this["options"]).val());
			return false;
	});

}

var app = {

	initialize: function() {
		this.bindEvents(); //binding event listeners to DOM in the app
		connectedPage.hidden = true; //hides the HTML elements for the second page
	},
	bindEvents: function() {
		document.addEventListener('deviceready', this.onDeviceReady, false); //runs onDeviceReady function whenever the device is ready (loaded)
		refreshButton.addEventListener('touchstart', this.refreshDeviceList, false); //on touch of the Refresh button, runs refreshDeviceList function
		deviceList.addEventListener('touchstart', this.connect, false); //on touch of device list, connect to device
		//make randomButton my new button pushToDevice!!1
		pushToDevice.addEventListener('touchstart', this.sendData, false);
		disconnectButton.addEventListener('touchstart', this.disconnect, false);
	},

	onDeviceReady: function() {
		app.refreshDeviceList();
		toggleButton();
	},

	refreshDeviceList: function() {
		deviceList.innerHTML = ''; // empties the list
		ble.scan([], 5, app.onDiscoverDevice, app.onError); //scans for BLE devices
	},

	onDiscoverDevice: function(device) {
		//only shows devices with the name we're looking for
		if(device.name === DEVICE) {
			//creates a HTML element to display in the app
			var listItem = document.createElement('li'),
			html = '<b>' + device.name + '</b><br/>' +
			'RSSI: ' + device.rssi + '&nbsp;|&nbsp;' +
			device.id;
			listItem.innerHTML = html;
			listItem.dataset.deviceId = device.id;         //save the device ID in the DOM element
			listItem.setAttribute("class", "result");      //give the element a class for css purposes
			deviceList.appendChild(listItem);              //attach it in the HTML element called deviceList
		}

	},

	connect: function(e) {
		//get the device ID from the DOM element
		var deviceId = e.target.dataset.deviceId,

		onConnect = function() {
			//saves device ID to buttons - needed later
			disconnectButton.dataset.deviceId = deviceId;
			pushToDevice.dataset.deviceId = deviceId;

			resultDiv.innerHTML = "Click to send data";

			//show next page -- no need
			//app.showConnectPage();
		};

		//connect functions asks for the device id, a callback function for when succeeds and one error functions for when it fails
		ble.connect(deviceId, onConnect, app.onError);
	},
	sendData: function() { //a function that calc random RGB and sends to device.-
    var deviceId = event.target.dataset.deviceId; // first find out device ID
		//then calc time
		var date = new Date();
		// returns the hours number for a date, between 1 and 12 {date.getHours()+ 24) % 12 || 12}
		var hours = date.getHours();
		var minutes = date.getMinutes();

		var data = new Uint8Array(4);
		data[0] = hours;
		data[1] = minutes;
		// data[2] = digit_3;
		// data[3] = digit_4;
		if(hours.toString().length == 1) hours = "" + 0 + hours;
		if(minutes.toString().length == 1) minutes = "" + 0 + minutes;

    	// send data to the bean
		ble.write(deviceId, scratchServiceUUID, scratchCharacteristicUUID, data.buffer, app.onSuccess(data), app.onError);
	},

	disconnect: function(event) {
		//gets device ID from disconnect button
		var deviceId = event.target.dataset.deviceId;
		ble.disconnect(deviceId, app.showconnectBlock, app.onError);
	},
	showStartPage: function() {
		startPage.hidden = false;
		connectedPage.hidden = true;
    batteryState.hidden = false;
	},
	showConnectPage: function() {
		startPage.hidden = true;
		connectedPage.hidden = false;
	},
	onError: function(reason) {
		alert("ERROR: " + reason); // real apps should use notification.alert
	},
	onSuccess: function(data){
    alert("Time stamp sent: " + data[0] + " : " + data[1] \n "Pattern: "+pattern[i]+ \n "Frequency: "+freq[i] \n "Time manipulation: "+ bool);
	}
};
