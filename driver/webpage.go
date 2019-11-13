package main

const webPage = `
<html>
	<head>
		<title>VGA2USB Userspace Liveview</title>
	</head>
	<body>
		<h1> This is a very basic live view, Please don't expect anything great </h1>
		<h2 id="status">Not Connecting</h2>
		<canvas width=800 height=600  id="liveview">This needs canvas, maybe upgrade browser if you can see this</canvas>
		<button id="record">Toggle Record</button>


		<script>
			// https://github.com/SMUsamaShah/CanvasRecorder
			function CanvasRecorder(canvas, video_bits_per_sec) {
				this.start = startRecording;
				this.stop = stopRecording;
				this.save = download;
			
				var recordedBlobs = [];
				var supportedType = null;
				var mediaRecorder = null;
			
				var stream = canvas.captureStream();
				if (typeof stream == undefined || !stream) {
					return;
				}
			
				const video = document.createElement('video');
				video.style.display = 'none';
			
				function startRecording() {
					let types = [
						"video/webm\;codecs=h264",
						'video/webm,codecs=vp9',
						"video/webm",
						'video/vp8',
						"video/webm\;codecs=vp8",
						"video/webm\;codecs=daala",
						"video/mpeg"

					];
			
					for (let i in types) {
						if (MediaRecorder.isTypeSupported(types[i])) {
							supportedType = types[i];
							break;
						}
					}
					if (supportedType == null) {
						console.log("No supported type found for MediaRecorder");
					}
					let options = { 
						mimeType: supportedType,
						videoBitsPerSecond: video_bits_per_sec || 2500000 // 2.5Mbps
					};
			
					recordedBlobs = [];
					try {
						mediaRecorder = new MediaRecorder(stream, options);
					} catch (e) {
						alert('MediaRecorder is not supported by this browser.');
						console.error('Exception while creating MediaRecorder:', e);
						return;
					}
			
					console.log('Created MediaRecorder', mediaRecorder, 'with options', options);
					mediaRecorder.onstop = handleStop;
					mediaRecorder.ondataavailable = handleDataAvailable;
					mediaRecorder.start(100); // collect 100ms of data blobs
					console.log('MediaRecorder started', mediaRecorder);
				}
			
				function handleDataAvailable(event) {
					if (event.data && event.data.size > 0) {
						recordedBlobs.push(event.data);
					}
				}
			
				function handleStop(event) {
					console.log('Recorder stopped: ', event);
					const superBuffer = new Blob(recordedBlobs, { type: supportedType });
					video.src = window.URL.createObjectURL(superBuffer);
				}
			
				function stopRecording() {
					mediaRecorder.stop();
					console.log('Recorded Blobs: ', recordedBlobs);
					video.controls = true;
				}
			
				function download(file_name) {
					const name = file_name || 'recording.webm';
					const blob = new Blob(recordedBlobs, { type: supportedType });
					const url = window.URL.createObjectURL(blob);
					const a = document.createElement('a');
					a.style.display = 'none';
					a.href = url;
					a.download = name;
					document.body.appendChild(a);
					a.click();
					setTimeout(() => {
						document.body.removeChild(a);
						window.URL.revokeObjectURL(url);
					}, 100);
				}
			}
		</script>
		<script>
			const canvas = document.getElementById('liveview');
			const statusBar = document.getElementById('status');
			const recorder = new CanvasRecorder(canvas);
			const context = canvas.getContext('2d');
			recordButton = document.getElementById("record");
			var recording = false;
			recordButton.onclick = function(){
				if (recording) {
					recorder.stop();
					recording=false;
					setTimeout(function(){

						recorder.save('vga2usb.webm');
					
					},1500)
				} else {
					recorder.start();
					recording=true;
				}
			}

			statusBar.innerHTML = "Connecting";
			var pngSocket = new WebSocket("ws://" + location.host + "/ws");
            pngSocket.onopen = function (event) {
				statusBar.innerHTML = "Connected?";
			};
            pngSocket.onmessage = function (event) {
				base_image = new Image();
				event.data.type = "image/png";

				base_image.src = URL.createObjectURL(event.data);
				
				base_image.onload = function(){
				  context.drawImage(base_image, 0, 0);
				}			  
            }
            pngSocket.onclose = function (event) {
				statusBar.innerHTML = "Failed to connect, or server closed";
            }

		</script>
	</body>
</html>
`
