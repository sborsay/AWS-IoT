t = Math.floor (Math.random() * (90-35) + 50)
h = Math.floor (Math.random() * (50-5) + 35)
c = Math.floor (Math.random() * (22-5) + 22)
hi = Math.floor (Math.random() * (33-5) + 33)

msg.payload = { temperature: t, celcius: c,  humidity: h,  humidityIndex: hi, timestamp:  msg.payload, }
return msg;