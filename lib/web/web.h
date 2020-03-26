/**
 * web.h
 * 
 * Web pages.
 * 
 * created by Tihomir Magdic
 * created 2020-3-21
 */

#ifndef WEB_H
#define WEB_H

#include <Arduino.h>

const char web_css[] PROGMEM = R"=====(
.menu {
  display: flex;
  align-items: center;
  width: 100%;
  font-size: 2em;
  margin-bottom: 1em;
  border-bottom: 1px solid #5a5a5a66;
  padding-bottom: 0.5em;
  font-family: OpenSans-Regular, sans-serif;
	height: 2em;
}

.title {
  color: #2871fa;
	font-size: 0.9em;
  font-weight: 700;
  width: 100%;
  text-align: center;
  height: 100%;
  flex-basis: 100%;
	align-items: center;
	display: flex;
	justify-content: center;
}

.wrap {
  margin: 0 auto;
  width: 500px;
}

.page {
  font-family: OpenSans-Regular, sans-serif;
  display: flex;
  flex-direction: column;
  apadding: 0;
  padding-left: 15px;
  padding-right: 15px;
  awidth: 100%;
  position: relative;
  awidth: 200px;
}

.item {
  awidth: 100%;
  margin-bottom: 1em;
}

.label {
  margin-bottom: 0.2em;
  color: #00000090;
}

.control {
  display: flex;
}

.input {
  background-color: #f7f7f7;
  border: 1px solid #e7e7e7;
  border-radius: 8px;
  width: 100%;
  font-size: 1em;
  height: 1.5em;
  apadding: 4px 4px;
  outline: none;
  box-shadow: none;
  padding: 0px 4px 4px 4px;
  -webkit-appearance: none;
  -moz-appearance: none;
  appearance: none;
}

.read-only, .input:focus {
  background-color: white; 
  border-color: none;
}

.center {
  text-align: center;
}

.json {
  font-size: 1.2em;
  font-family: monospace;
  white-space: pre;
}

.tool {
  margin-top: 1em;
}

.button {
  width: 100%;
  border-radius: 8px;
	font-size: 1.2em;
  height: 3em;
  outline: none;
  border: none;
  background-color: #2871fa;
  color: white;
  font-weight: 700;
  cursor: pointer;
}

.back {
  padding: 8px;
  border: 1px solid #54525254;
  cursor: pointer;
  background-color: #2871fa;
  color: white;
  font-weight: 700;
  border-radius: 4px;
  font-size: 1.8rem;
  width: 2em;
  height: 4rem;
  display: flex;
  text-align: center;
  justify-content: center;
}

.wifilist {
  margin-bottom: 1em;
  border-bottom: 1px solid #3c3c3c45;
}

.wifiheader {
  display: flex;
  align-items: center;
  border-bottom: 1px solid #3c3c3c45;
  padding-bottom: 0.5em;
  margin-bottom: 0.5em;
  border-width: 1px;
}

.scanning {
	display: block;
	text-align: center;
  margin-bottom: 0.5em;
}

.noscanning {
	display: none;
}

.wifi {
  font-weight: 700;
  font-size: 1em;
  min-width: 70%;
}

.row {
  display: flex;
  flex-direction: column;
  font-size: 0.8em;
  amargin-bottom: 0.8em;
	padding: 0.5em;
	cursor: pointer;
}

.row:hover {
  background-color: #ececec;
}

.net {
  display: flex;
  flex-direction: row;
  width: 100%;
}

.ssid {
  width: 50%;
  color: blue;
  font-weight: 700;
  overflow-x: hidden;
  white-space: nowrap;
  text-overflow: ellipsis;
  padding-right: 0.5em;
}

.strength {
  width: 25%;
  padding-right: 0.5em;
}

.security {
  width: 25%;
}

.detail {
  display: flex;
  flex-direction: row;
  width: 100%;
  justify-content: flex-end;
}

.mac {
  white-space: nowrap;
}

.channel {
  white-space: nowrap;
  margin-left: 1em;
}

.test {
	background-color: #162c7b;
}

.reset {
	background-color: red;
}

.error {
	color: red;
}

.noerror {
	display: none;
}

@media (max-width: 576px) {
  .wrap {
    width: auto;
  }
  .page {
    apadding-left: 15px;
    apadding-right: 15px;
    padding: 0;
  }
}
)=====";

const String web_js = R"=====(
function g(i) {
	return document.getElementById(i);
};
function E(s) {
	return document.createElement(s)
};
var
	S = "setAttribute",
	A = "appendChild",
	H = "innerHTML",
	X = new XMLHttpRequest(), wl, sc, sent = false;
function scan() {
	if (sent)
	  return;
	wl = g("wifilist");
	document.querySelectorAll('.row').forEach(function(row){
		row.remove();
	});
	sc = g("scanning");
	sc[S]("class", "scanning");
	X.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			sc[S]("class", "noscanning");
			var nets = JSON.parse(this.responseText);
			var row, net, ssid, strength, security, detail, mac, channel;
			for(var i = 0; i < nets.length; i++) {
				row = E("div");
				row[S]("class", "row");
				let ssidvalue = nets[i].ssid;
				row.onclick = function() {
					g('ssid').value = ssidvalue;
					g('pass').value = "";
					g('pass').focus();
				};
				wl[A](row); 

				net = E("div");
				net[S]("class", "net");
				row[A](net); 

				ssid = E("span");
				ssid[S]("class", "ssid");
				ssid[H] = nets[i].ssid;
				net[A](ssid); 

				strength = E("span");
				strength[S]("class", "strength");
				strength[H] = nets[i].strength + "%";
				net[A](strength); 

				security = E("span");
				security[S]("class", "security");
				security[H] = nets[i].security;
				net[A](security); 

				detail = E("div");
				detail[S]("class", "detail");
				row[A](detail); 

				mac = E("span");
				mac[S]("class", "mac");
				mac[H] = "MAC: " + nets[i].mac;
				detail[A](mac); 

				channel = E("span");
				channel[S]("class", "channel");
				channel[H] = "Ch: " + nets[i].channel;
				detail[A](channel);
			}
			sent = false;
		}
	}
	X.open("GET", "wifi.json", true);
	X.send();
	sent = true;
}
scan();
)=====";

//const char web_menu[] PROGMEM = R"=====(
const String web_menu = R"=====(
<!DOCTYPE html>
<html>
  <head>
		<meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>{{ap-name}}</title>
		<link href="style.css" type="text/css" rel="stylesheet">
  </head>
  <body>

    <div class="wrap">
			<div class="menu">
				<span class="title">{{ap-name}}</span>
			</div>
      <div class="page">

        <div class="tool">
          <button class="button" onclick="window.location.href = '/status';">STATUS</button>
        </div>

        <div class="tool">
          <button class="button" onclick="window.location.href = '/configuration';">NETWORK CONFIGURATION</button>
        </div>

        <div class="tool">
          <button class="button" onclick="window.location.href = '/reset';">RESET</button>
        </div>

      </div>
    </div>

  </body>
</html>
)=====";

//const char web_status[] PROGMEM = R"=====(
const String web_status = R"=====(
<!DOCTYPE html>
<html>
  <head>
		<meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>{{ap-name}}</title>
		<link href="style.css" type="text/css" rel="stylesheet">
  </head>
  <body>

		<div class="wrap">
			<div class="menu">
				<button class="button back" onclick="window.location.href = '/';">&lt;</button>
				<span class="title">Status</span>
			</div>
			<div class="page">

				<div class="item">
					<div class="label">Device name</div>
					<div class="control">
						<input class="input read-only" readonly value="{{device}}"/>
					</div>
				</div>

				<div class="item">
					<div class="label">Firmware</div>
					<div class="control">
						<input class="input read-only" readonly value="{{firmware-version}}"/>
					</div>
				</div>

				<div class="item">
					<div class="label">Configuration</div>
					<div class="control">
						<input class="input read-only" readonly value="{{config-version}}"/>
					</div>
				</div>

				<div class="item">
					<div class="label">Last data</div>
					<div class="control">
						<div class="json">{{last-data}}</div>
					</div>
				</div>

				<div class="item">
					<div class="label">Last connection</div>
					<div class="control">
						<div class="value">{{last-connection}}</div>
					</div>
				</div>

			</div>
		</div>

  </body>
</html>
)=====";

//const char web_configuration[] PROGMEM = R"=====(
const String web_configuration = R"=====(
<!DOCTYPE html>
<html>
  <head>
		<meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>{{ap-name}}</title>
		<link href="style.css" type="text/css" rel="stylesheet">
		<script type="text/javascript" src="script.js" defer></script>
  </head>
  <body>

		<div class="wrap">
			<div class="menu">
				<button class="button back" onclick="window.location.href = '/';">&lt;</button>
				<span class="title">Configuration</span>
			</div>
			<div class="page">

			<div>
				<form action="/save">
					<div class="item">
						<div class="label">API key</div>
						<div class="control">
							<input class="input" name="apikey" value="{{apikey}}"/>
						</div>
					</div>

					<div id="wifilist" class="wifilist">
						<div class="wifiheader">
							<div class="wifi">Wifi networks</div>
							<button class="button" type="button" onclick="scan();">RESCAN</button>
						</div>
						<div id="scanning" class="noscanning">scanning...</div>
						<div style="display: none;" class="row">
							<div class="net">
								<span class="ssid">XXH0VH_092E</span>
								<span class="strength">-70dBm</span>
								<span class="security">WPA/WPA2/PSK</span>
							</div>
							<div class="detail">
								<span class="mac">MAC: 54:A1:FA:BB:29:5E</span>
								<span class="channel">Ch: 1</span>
							</div>
						</div>
					</div>

					<div class="item">
						<div class="label">SSID</div>
						<div class="control">
							<input class="input" id="ssid" name="ssid" value="{{wifissid}}"/>
						</div>
					</div>

					<div class="item">
						<div class="label">Password</div>
						<div class="control">
							<input class="input" id="pass" name="pass" value="{{wifipassword}}"/>
						</div>
					</div>

					<div class="tool">
						<button class="button" type="submit">SAVE</button>
					</div>
				</form>
			</div>

			</div>
		</div>

  </body>
</html>
)=====";

const char web_reset[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
		<meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>{{ap-name}}</title>
		<link href="style.css" type="text/css" rel="stylesheet">
  </head>
  <body>

		<div class="wrap">
			<div class="menu">
				<button class="button back" onclick="window.location.href = '/';">&lt;</button>
				<span class="title">Reset</span>
			</div>
			<div class="page">

				<div class="item">
					<div class="label center">Click on CONFIRM button to reset the device.</div>
				</div>

				<div class="tool">
					<button class="button reset" onclick="window.location.href = '/reset-now';">CONFIRM</button>
				</div>

			</div>
		</div>

  </body>
</html>
)=====";

#endif
