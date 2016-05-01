
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

console.log("create app")

var app = {

	initialize: function() {
		this.bindEvents(); //binding event listeners to DOM in the app
		connectedPage.hidden = true; //hides the HTML elements for the second page
	},

	bindEvents: function() {

			var that = this;

			function callSendData(_test){

				that.sendData(_test)

			};


		document.addEventListener('deviceready', this.onDeviceReady, false); //runs onDeviceReady function whenever the device is ready (loaded)
		refreshButton.addEventListener('touchstart', this.refreshDeviceList, false); //on touch of the Refresh button, runs refreshDeviceList function
		deviceList.addEventListener('touchstart', this.connect, false); //on touch of device list, connect to device
		//make randomButton my new button pushToDevice!!1
		pushToDevice.addEventListener('touchstart',this.SendData,false);
		//pushToDevice.addEventListener('touchstart', function(){
			// 	console.log("push")
			// that.SendData(false);
			// },false);
		testDevice.addEventListener('touchstart', function(){
			console.log("test")
				that.SendData(true);
				},false);
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
		console.log("SEDNDATA function");
    var deviceId = event.target.dataset.deviceId; // first find out device ID
		//then calc time
		var date = new Date();
		// returns the hours number for a date, between 1 and 12 {date.getHours()+ 24) % 12 || 12}
		var hours = date.getHours();
		var minutes = date.getMinutes();

		//convert to ms:
		var completeTime = (hours * 60 * 60 * 1000) + (minutes * 60 * 1000) + ""; //"" makes it a string

		while(completeTime.length < 8){ //mke sure 8 digits (add zeros)
			completeTime = "0" + completeTime;
		};

		//slice string to 3 parts
		var digit_12 = completeTime.slice(0,2); //start to end pos
		var digits_345 = completeTime.slice(2,5);
		var digits_678 = completeTime.slice(5,8);

		//var data = new Uint8Array(4); //change this to unsigned long
		var data = new Uint8Array(4); //9 bits for max of 86,399,000
		//highest we can get is 23:59 based on: 24*60*60*1000-1000 = 86,399,000

		/////////-----------------------first digit
	  data[0] = digit_12;  //0 to 86

	  //////---------------------- 234 digits
		//devide each part to by the corrisponding amoutof bits
		var fullBits = Math.floor(digits_345 / 255);
		//mudule the spliced number in 255
		var restBit = digits_678 % 255;

		data[1] = fullBits;  //multiply by 255 and add data[2] in arduino side
		data[2] = restBit;
		// if(_test){
		// 	data[3] = 1; //test always off here
		// }else{
		// 	data[3] = 0; //test always off here
		// }


		//more data to be send from phone:
		var testButton = 1;
		var pattern = "Moving point";
		var freq = "5 min";
		var bool = "No";

    	// send data to the bean
		console.log(data);
		console.log("im here!!!");
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
    alert("Time stamp in ms: " + data[0] + data[1] + data[2] +"\n"+ "Pattern: "+"pattern"+ "\n"+"Frequency: "+"freq"+"\n"+"Time manipulation: "+ "bool");
	}
};
