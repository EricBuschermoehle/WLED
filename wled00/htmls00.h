/*
 * Index html
 */
const char PAGE_index[] PROGMEM = R"=====(
<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
  <title>WLED 0.3pd</title>
  <script>
      strA = "";
    strR = "";
    strG = "";
    strB = "";
    
    function Startup()
    {
      setInterval('GetArduinoIO()', 5000);
      GetArduinoIO();
    }
    function GetArduinoIO()
    {
      nocache = "&nocache=" + Math.random() * 1000000;
      var request = new XMLHttpRequest();
      request.onreadystatechange = function()
      {
        if (this.readyState == 4) {
          if (this.status == 200) {
            if (this.responseXML != null) {
              document.Ctrl_form.SA.value = this.responseXML.getElementsByTagName('act')[0].childNodes[0].nodeValue;
              document.Ctrl_form.SR.value = this.responseXML.getElementsByTagName('cl')[0].childNodes[0].nodeValue;
              document.Ctrl_form.SG.value = this.responseXML.getElementsByTagName('cl')[1].childNodes[0].nodeValue;
              document.Ctrl_form.SB.value = this.responseXML.getElementsByTagName('cl')[2].childNodes[0].nodeValue;
              UpdateVals();
            }
          }
        }
      }
      // send HTTP request
      request.open("GET", "win/" + strA + strR + strG + strB + nocache, true);
      request.send(null);
      strA = "";
      strR = "";
      strG = "";
      strB = "";
    }
    function GetCheck()
    {
      
      strA = "&A=" + Ctrl_form.SA.value;
      strR = "&R=" + Ctrl_form.SR.value;
      strG = "&G=" + Ctrl_form.SG.value;
      strB = "&B=" + Ctrl_form.SB.value;
      
      UpdateVals();
      GetArduinoIO();
    }
    function rgb2hex(red, green, blue) {
      var rgb = blue | (green << 8) | (red << 16);
      return '#' + (0x1000000 + rgb).toString(16).slice(1)
    }
    function lingrad(red, green, blue) {
      return "linear-gradient(white, " + rgb2hex(red, green, blue) + ")";
    }
    function UpdateVals()
    {
      document.body.style.background = lingrad(Ctrl_form.SR.value, Ctrl_form.SG.value, Ctrl_form.SB.value);
    }
    function OpenSettings()
    {
      window.open("/settings","_self");
    }
    function ToggleNightMode()
    {
    
    }
  </script>
  <style>
    .ctrl_box {
      border: 0.3ch solid grey;
      margin: auto;
      width: 80vw;
      background-color: #B9B9B9;
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
    }
    .sliders {
      width: 100%;
      height: 12vh;
    }
    .sliderA {
      margin-left: auto;
      margin-right: auto;
      width: 77vw;
      background: linear-gradient(to right, black, yellow);
    }
    .sliderR {
      margin-left: auto;
      margin-right: auto;
      width: 77vw;
      background: linear-gradient(to right, black, red);
    }
    .sliderG {
      margin-left: auto;
      margin-right: auto;
      width: 77vw;
      background: linear-gradient(to right, black, green);
    }
    .sliderB {
      margin-left: auto;
      margin-right: auto;
      width: 77vw;
      background: linear-gradient(to right, black, blue);
    }
    body {
      text-align: right;
      background: linear-gradient(white, black);
      height: 100%;
      margin: 0;
      background-repeat: no-repeat;
      background-attachment: fixed;
    }
    html {
      height: 100%;
    }
    .tool_box {
      
    }
    input[type=image] {
      width: 10vmin;
      height: 10vmin;
      margin-top: 4px;
      margin-right: 4px;
    }
    input[type=range] {
      -webkit-appearance: none;
      width: 100%;
      margin: -4px 0;
    }
    input[type=range]:focus {
      outline: none;
    }
    input[type=range]::-webkit-slider-runnable-track {
      width: 100%;
      height: 12vh;
      cursor: pointer;
      background: #DDDDDD;
    }
    input[type=range]::-webkit-slider-thumb {
      height: 10vh;
      width: 10vh;
      background: #ffffff;
      cursor: pointer;
      -webkit-appearance: none;
      margin-top: 1vh;
    }
    input[type=range]::-moz-range-track {
      width: 100%;
      height: 12vh;
      cursor: pointer;
      background: #DDDDDD;
    }
    input[type=range]::-moz-range-thumb {
      height: 10vh;
      width: 10vh;
      background: #ffffff;
      cursor: pointer;
      margin-top: 1vh;
    }
    input[type=range]::-ms-track {
      width: 100%;
      height: 12vh;
      cursor: pointer;
      background: transparent;
      border-color: transparent;
      color: transparent;
    }
    input[type=range]::-ms-fill-lower {
      background: #DDDDDD;
    }
    input[type=range]::-ms-fill-upper {
      background: #DDDDDD;
    }
    input[type=range]::-ms-thumb {
      width: 10vh;
      background: #ffffff;
      cursor: pointer;
      height: 10vh;
    }
  </style>
<style id="holderjs-style" type="text/css"></style></head>
<body onload="Startup()" class=" __plain_text_READY__">
  <div id="tbB" class="tool_box">
    <input type="image" class="settingsbutton" src="/button.png" onclick="OpenSettings()"id="tool">
    <!--<input type="image" class="nightbutton" src="/moon.png" onclick="ToggleNightMode()"id="night">-->
  </div>
  <div id="cdB" class="ctrl_box">
    <form id="form_c" name="Ctrl_form">
      <br>
      <div id="slA" class="sliderA">
      <input type="range" class="sliders" name="SA" value="0" min="0" max="255" step="1" onchange="GetCheck()"> </div> <br>
      <div id="slR" class="sliderR">
      <input type="range" class="sliders" name="SR" value="0" min="0" max="255" step="1" onchange="GetCheck()"> </div> <br>
      <div id="slR" class="sliderG">
      <input type="range" class="sliders" name="SG" value="0" min="0" max="255" step="1" onchange="GetCheck()"> </div> <br>
      <div id="slR" class="sliderB">
      <input type="range" class="sliders" name="SB" value="0" min="0" max="255" step="1" onchange="GetCheck()"> </div> <br>
    </form> 
  </div>
</div></body></html>
)=====";
/*
 * SPIFFS editor html
 */
const char PAGE_edit[] PROGMEM = R"=====(
<!DOCTYPE html><html lang="en"><head><title>ESP Editor</title><style type="text/css" media="screen">.cm{z-index:300;position:absolute;left:5px;border:1px solid #444;background-color:#F5F5F5;display:none;box-shadow:0 0 10px rgba(0,0,0,.4);font-size:12px;font-family:sans-serif;font-weight:700}.cm ul{list-style:none;top:0;left:0;margin:0;padding:0}.cm li{position:relative;min-width:60px;cursor:pointer}.cm span{color:#444;display:inline-block;padding:6px}.cm li:hover{background:#444}.cm li:hover span{color:#EEE}.tvu ul,.tvu li{padding:0;margin:0;list-style:none}.tvu input{position:absolute;opacity:0}.tvu{font:normal 12px Verdana,Arial,Sans-serif;-moz-user-select:none;-webkit-user-select:none;user-select:none;color:#444;line-height:16px}.tvu span{margin-bottom:5px;padding:0 0 0 18px;cursor:pointer;display:inline-block;height:16px;vertical-align:middle;background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAAK/INwWK6QAAABl0RVh0U29mdHdhcmUAQWRvYmUgSW1hZ2VSZWFkeXHJZTwAAADoSURBVBgZBcExblNBGAbA2ceegTRBuIKOgiihSZNTcC5LUHAihNJR0kGKCDcYJY6D3/77MdOinTvzAgCw8ysThIvn/VojIyMjIyPP+bS1sUQIV2s95pBDDvmbP/mdkft83tpYguZq5Jh/OeaYh+yzy8hTHvNlaxNNczm+la9OTlar1UdA/+C2A4trRCnD3jS8BB1obq2Gk6GU6QbQAS4BUaYSQAf4bhhKKTFdAzrAOwAxEUAH+KEM01SY3gM6wBsEAQB0gJ+maZoC3gI6iPYaAIBJsiRmHU0AALOeFC3aK2cWAACUXe7+AwO0lc9eTHYTAAAAAElFTkSuQmCC) no-repeat;background-position:0 0}.tvu span:hover{text-decoration:underline}@media screen and (-webkit-min-device-pixel-ratio:0){.tvu{-webkit-animation:webkit-adjacent-element-selector-bugfix infinite 1s}}@-webkit-keyframes webkit-adjacent-element-selector-bugfix{from{padding:0}to{padding:0}}#uploader{position:absolute;top:0;right:0;left:0;height:28px;line-height:24px;padding-left:10px;background-color:#444;color:#EEE}#tree{position:absolute;top:28px;bottom:0;left:0;width:160px;padding:8px}#editor,#preview{position:absolute;top:28px;right:0;bottom:0;left:160px;border-left:1px solid #EEE}#preview{background-color:#EEE;padding:5px}</style><script>
eval(function(p,a,c,k,e,r){e=function(c){return(c<a?'':e(parseInt(c/a)))+((c=c%a)>35?String.fromCharCode(c+29):c.toString(36))};if(!''.replace(/^/,String)){while(c--)r[e(c)]=k[c]||e(c);k=[function(e){return r[e]}];e=function(){return'\\w+'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c]);return p}('3 2d(d,f,g){5 h;5 i=6.r("2e");i.1a="1e";i.2o=w;i.G="1C";6.v(d).s(i);5 j=6.r("2e");j.K="3h-17";j.1a="13";j.G="17";j.2G="/";6.v(d).s(j);5 k=6.r("2i");k.J=\'2t\';6.v(d).s(k);5 l=6.r("2i");l.J=\'37\';6.v(d).s(l);3 10(){7(h.Y==4){7(h.B!=W)1t("1s["+h.B+"]: "+h.U);L{f.1E(j.t)}}}3 1H(p){h=u T();h.R=10;5 a=u 1c();a.1d("17",p);h.P("2s","/X");h.Q(a)}l.H=3(e){7(j.t.3n(".")===-1)q;1H(j.t);g.15(j.t)};k.H=3(e){7(i.16.C===0){q}h=u T();h.R=10;5 a=u 1c();a.1d("1C",i.16[0],j.t);h.P("1M","/X");h.Q(a)};i.3j=3(e){7(i.16.C===0)q;5 a=i.16[0].G;5 b=/(?:\\.([^.]+))?$/.E(a)[1];5 c=/(.*)\\.[^.]+$/.E(a)[1];7(z c!==y){a=c}7(z b!==y){7(b==="1p")b="1b";L 7(b==="3c")b="1N";a=a+"."+b}7(j.t==="/"||j.t.1X("/")===0){j.t="/"+a}L{j.t=j.t.3k(0,j.t.1X("/")+1)+a}}}3 25(k,l){5 m=6.v("29");5 n=6.r("2f");n.2g="2I";6.v(k).s(n);3 2h(a){6.v(\'2n-3f\').2l=a+"?2n=A"}3 1m(a){6.v("1l").D.O="1V";m.D.O="18";m.J=\'<2y 2l="\'+a+\'?2B=\'+2E.2F()+\'" D="20-2H:22%; 20-2K:22%; 2R:2Y; O:18;" />\'}3 23(a,b){5 c=6.r("24");a.s(c);5 d=6.r("19");c.s(d);7(1k(b)){d.J="<x>3x</x>";d.H=3(e){l.15(b);7(6.9.11(\'N\').C>0)6.9.I(a)}}L 7(1q(b)){d.J="<x>2u</x>";d.H=3(e){1m(b);7(6.9.11(\'N\').C>0)6.9.I(a)}}5 f=6.r("19");c.s(f);f.J="<x>2v</x>";f.H=3(e){2h(b);7(6.9.11(\'N\').C>0)6.9.I(a)};5 g=6.r("19");c.s(g);g.J="<x>2w</x>";g.H=3(e){2j(b);7(6.9.11(\'N\').C>0)6.9.I(a)}}3 2k(e,a,b){5 c=6.r("2f");5 d=6.9.1f?6.9.1f:6.1D.1f;5 f=6.9.1g?6.9.1g:6.1D.1g;5 g=1F.1h+f;5 h=1F.1i+d;c.2g=\'N\';c.D.O=\'18\';c.D.2Q=g+\'1I\';c.D.2T=h+\'1I\';23(c,a);6.9.s(c);5 i=c.2U;5 j=c.2W;c.2X=3(e){7(e.1h<g||e.1h>(g+i)||e.1i<h||e.1i>(h+j)){7(6.9.11(\'N\').C>0)6.9.I(c)}}}3 1J(a,b,c){5 d=6.r("19");d.K=(((a=="/")?"":a)+"/"+b);5 f=6.r("x");f.30=b;d.s(f);d.H=3(e){7(1k(d.K.1K())){l.15(d.K)}L 7(1q(d.K.1K())){1m(d.K)}};d.38=3(e){e.3a();e.3b();2k(e,d.K,A)};q d}3 1L(a,b,c){5 d=6.r("24");a.s(d);5 e=c.C;3d(5 i=0;i<e;i++){7(c[i].1a==="1e")d.s(1J(b,c[i].G,c[i].3e))}}3 1k(a){5 b=/(?:\\.([^.]+))?$/.E(a)[1];7(z b!==y){1j(b){8"1O":8"1b":8"1P":8"c":8"1Q":8"1R":8"1S":q A}}q w}3 1q(a){5 b=/(?:\\.([^.]+))?$/.E(a)[1];7(z b!==y){1j(b){8"2q":8"1N":8"2r":q A}}q w}1T.1E=3(a){n.I(n.1U[0]);F(n,"/")};3 1W(a){q 3(){7(o.Y==4){7(o.B!=W){1t("1s["+o.B+"]: "+o.U)}L{n.I(n.1U[0]);F(n,"/")}}}}3 2j(a){o=u T();o.R=1W(a);5 b=u 1c();b.1d("17",a);o.P("2x","/X");o.Q(b)}3 1Y(a,b){q 3(){7(o.Y==4){7(o.B==W)1L(a,b,2z.2A(o.U))}}}3 F(a,b){o=u T(a,b);o.R=1Y(a,b);o.P("1Z","/2C?2D="+b,A);o.Q(1n)}F(n,"/");q 1T}3 21(e,f,g,h,i){3 1o(a){5 b="V";5 c=/(?:\\.([^.]+))?$/.E(a)[1];7(z c!==y){1j(c){8"1O":b="V";12;8"1b":b="1p";12;8"1P":b="2J";12;8"c":b="1r";12;8"1Q":b="1r";12;8"1R":8"2L":8"2M":8"1p":8"2N":8"1S":b=c}}q b}7(z f==="y")f="/2O.1b";7(z g==="y"){g=1o(f)}7(z h==="y")h="2P";7(z i==="y"){i="13/"+g;7(g==="1r")i="13/V"}5 j=1n;5 k=14.X(e);3 10(){7(j.Y==4){7(j.B!=W)1t("1s["+j.B+"]: "+j.U)}}3 26(a,b,c){j=u T();j.R=10;5 d=u 1c();d.1d("1C",u 2S([b],{1a:c}),a);j.P("1M","/X");j.Q(d)}3 27(){7(j.Y==4){6.v("29").D.O="1V";6.v("1l").D.O="18";7(j.B==W)k.28(j.U);L k.28("");k.2V()}}3 F(a){j=u T();j.R=27;j.P("1Z",a,A);j.Q(1n)}7(g!=="V")k.M().2a("14/2b/"+g);k.2Z("14/2c/"+h);k.$31=32;k.M().33(A);k.M().34(2);k.35(A);k.36(w);k.1u.1v({G:\'39\',1w:{1x:\'1y-S\',1z:\'1A-S\'},E:3(a){26(f,a.3g()+"",i)},1B:w});k.1u.1v({G:\'3i\',1w:{1x:\'1y-Z\',1z:\'1A-Z\'},E:3(a){a.M().2m().3l(w)},1B:w});k.1u.1v({G:\'3m\',1w:{1x:\'1y-1G-Z\',1z:\'1A-1G-Z\'},E:3(a){a.M().2m().3o(w)},1B:w});F(f);k.15=3(a){f=a;g=1o(f);i="13/"+g;7(g!=="V")k.M().2a("14/2b/"+g);F(f)};q k}3 3p(){5 c={};5 d=3q.3r.3s.3t(/[?&]+([^=&]+)=([^&]*)/3u,3(m,a,b){c[a]=b});5 e=21("1l",c.1e,c.3v,c.2c);5 f=25("3w",e);2d("2p",f,e)};',62,220,'|||function||var|document|if|case|body|||||||||||||||xmlHttp||return|createElement|appendChild|value|new|getElementById|false|span|undefined|typeof|true|status|length|style|exec|httpGet|name|onclick|removeChild|innerHTML|id|else|getSession|cm|display|open|send|onreadystatechange||XMLHttpRequest|responseText|plain|200|edit|readyState||httpPostProcessRequest|getElementsByClassName|break|text|ace|loadUrl|files|path|block|li|type|htm|FormData|append|file|scrollTop|scrollLeft|clientX|clientY|switch|isTextFile|editor|loadPreview|null|getLangFromFilename|html|isImageFile|c_cpp|ERROR|alert|commands|addCommand|bindKey|win|Ctrl|mac|Command|readOnly|data|documentElement|refreshPath|event|Shift|createPath|px|createTreeLeaf|toLowerCase|addList|POST|jpg|txt|js|cpp|css|xml|this|childNodes|none|delCb|lastIndexOf|getCb|GET|max|createEditor|100|fillFileMenu|ul|createTree|httpPost|httpGetProcessRequest|setValue|preview|setMode|mode|theme|createFileUploader|input|div|className|loadDownload|button|httpDelete|showContextMenu|src|getUndoManager|download|multiple|uploader|png|gif|PUT|Upload|Preview|Download|Delete|DELETE|img|JSON|parse|_cb|list|dir|Date|now|defaultValue|width|tvu|javascript|height|scss|php|json|index|textmate|left|margin|Blob|top|offsetWidth|clearSelection|offsetHeight|onmouseout|auto|setTheme|innerText|blockScrolling|Infinity|setUseSoftTabs|setTabSize|setHighlightActiveLine|setShowPrintMargin|Create|oncontextmenu|saveCommand|preventDefault|stopPropagation|jpeg|for|size|frame|getValue|upload|undoCommand|onchange|substring|undo|redoCommand|indexOf|redo|onBodyLoad|window|location|href|replace|gi|lang|tree|Edit'.split('|'),0,{}))
</script><script src="https://cdnjs.cloudflare.com/ajax/libs/ace/1.1.9/ace.js" type="text/javascript" charset="utf-8"></script></head><body onload="onBodyLoad();"><div id="uploader"></div><div id="tree"></div><div id="editor"></div><div id="preview" style="display:none;"></div><iframe id=download-frame style='display:none;'></iframe></body></html>
)=====";
