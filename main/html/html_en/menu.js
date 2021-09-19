function menu(page_name)
{
var l='<div id="logo" style="padding-right:30px"><table style="width:100%">';
l+='<tr>';
l+='<td style="vertical-align:bottom;text-align:left;border:0px"><h1>'+devname+'</h1><br>'+page_name;
l+='<td style="vertical-align:bottom;text-align:right;border:0px;font-size:80%">';
l+=fwver+' / HW 1.'+hwver;
if(sys_name) l+='<br>';
l+=sys_name;
if(sys_location) l+='<br>';
l+=sys_location.replace("<","&lt;");
l+='</table></div>';
l+='<div id="menu">';
l+=[
'<a href="index.html">Home</a>',
'<a href="settings.html">Setup</a>',
'<a href="sendmail.html">E-mail</a>',
'<a href="io.html">Discrete IO</a>',
'<a href="wdog.html">Watchdog</a>',
'<a href="logic.html">Logic</a>',
'<a href="update.html">Firmware</a>',
'<a href="log.html">Log</a>'
].join(' | ');
l+='</div>';
document.write(l);
}

function hexb(v,len) //v3
{
 if(!len) len=1;
 var r='',b;
 for(n=0;n<len;++n)
 {
  b=v&0xff; v>>=8; // little endian pack of v
  if(b<0x10) r+='0';
  r+=b.toString(16);
 }
 return r;
}

var w1251table = {
0x0402:0x80,
0x0403:0x81,
0x201A:0x82,
0x0453:0x83,
0x201E:0x84,
0x2026:0x85,
0x2020:0x86,
0x2021:0x87,
0x20AC:0x88,
0x2030:0x89,
0x0409:0x8A,
0x2039:0x8B,
0x040A:0x8C,
0x040C:0x8D,
0x040B:0x8E,
0x040F:0x8F,
0x0452:0x90,
0x2018:0x91,
0x2019:0x92,
0x201C:0x93,
0x201D:0x94,
0x2022:0x95,
0x2013:0x96,
0x2014:0x97,
0x2122:0x99,
0x0459:0x9A,
0x203A:0x9B,
0x045A:0x9C,
0x045C:0x9D,
0x045B:0x9E,
0x045F:0x9F,
0x00A0:0xA0,
0x040E:0xA1,
0x045E:0xA2,
0x0408:0xA3,
0x00A4:0xA4,
0x0490:0xA5,
0x00A6:0xA6,
0x00A7:0xA7,
0x0401:0xA8,
0x00A9:0xA9,
0x0404:0xAA,
0x00AB:0xAB,
0x00AC:0xAC,
0x00AD:0xAD,
0x00AE:0xAE,
0x0407:0xAF,
0x00B0:0xB0,
0x00B1:0xB1,
0x0406:0xB2,
0x0456:0xB3,
0x0491:0xB4,
0x00B5:0xB5,
0x00B6:0xB6,
0x00B7:0xB7,
0x0451:0xB8,
0x2116:0xB9,
0x0454:0xBA,
0x00BB:0xBB,
0x0458:0xBC,
0x0405:0xBD,
0x0455:0xBE,
0x0457:0xBF
};

function w1251(c) {
if(c<128) return c; //eng
if(c>=0x410&&c<=0x44f) return c-0x410+192;
if(c in w1251table) return w1251table[c];
return 63; //'?'
}

function pack(fmt, data) {
 var d=new Array();
 var i,l,n,x;
 for(n=0;n<fmt.__len;++n) d[n]='00';
 for(i in fmt) {
 if(typeof(data[i])=='undefined') { continue; }
  x=data[i]; f=fmt[i]; 
  switch(typeof x) 
  {
  case 'string':
   x=x.replace(/\"/g,'');
   l=x.length; if(l>f.len-2) l=f.len-2;
   d[f.offs]=hexb(l);
   for(n=0;n<l;++n) d[f.offs+n+1]=hexb(w1251(x.charCodeAt(n)));
   break;
  case 'object':
   if(!(x instanceof Array)) break;
   for(n=0;n<f.len;++n) { d[f.offs+n]=hexb(Number(x[n])); }
   break;
  default:
   x=Number(x); 
   for(n=0;n<f.len;++n) { d[f.offs+n]=hexb(x&0xFF); x>>=8; }
   break;
  }
 }
 return d.join('');
}

function showerr(fld,e)
{
 fld.style.backgroundColor=e?"#ffaaaa":"";
}

function seterr(fld,e)
{
 showerr(fld,e);
 if(e) throw fld;
}

function chk_passwd(fld)
{
 var v=fld.value;
 seterr(fld,v.match(/[:,"]/));
 return v;
}

function range(fld,a,b) // v4 leading zero aware
{
 v=parseInt(fld.value,10); seterr(fld,isNaN(v)||v<a||v>b); return v;
}

function expand_table(table_container, n_ch)
{
 var mch=/_ch_/g, mch1=/_ch1_/g;
 var rn,r,n,t,table=table_container.children[0];
 for(rn=0;rn<table.rows.length;++rn)
 {
  r=table.rows[rn];
  for(n=1;n<n_ch;n++)
   r.appendChild(r.cells[1].cloneNode(true));
 }
 t=table_container.innerHTML;
 n=0; t=t.replace(mch, function(){return n++ % n_ch;} );
 n=0; t=t.replace(mch1, function(){return n++ % n_ch + 1;} );
 table_container.innerHTML=t;
}

function set_nf_cb(ev,msk)
{
 f=document.notif_frm;
 f['nf_log'+ev].checked=msk&1;
 f['nf_syslog'+ev].checked=msk&2;
 f['nf_email'+ev].checked=msk&4;
 ///f['nf_sms'+ev].checked=msk&8;
 f['nf_trap'+ev].checked=msk&16; 
}

function get_nf_cb_hex(ev)
{
 f=document.notif_frm;
 msk=0;
 if(f['nf_log'+ev].checked) msk|=1;
 if(f['nf_syslog'+ev].checked) msk|=2;
 if(f['nf_email'+ev].checked) msk|=4;
 ///if(f['nf_sms'+ev].checked) msk|=8;
 if(f['nf_trap'+ev].checked) msk|=16;
 return hexb(msk,2);
}

function nf_cb_changed()
{
 var els=document.notif_frm.elements,i,e;
 var cnt=0,cnton=0;
 for(i=0;i<els.length;++i)
 {
  e=els[i];
  if((e.name.indexOf('nf_')==0)&&!e.disabled) { ++cnt; if(e.checked) ++cnton; }
 }
 var aocb=document.notif_frm.atonce;
 if(cnton==cnt) { aocb.checked=true; aocb.indeterminate=false; }
 else if(cnton==0) { aocb.checked=false; aocb.indeterminate=false; }
 else aocb.indeterminate=true;
}
