        ��  ��                  �  @   D A T A   P L U G I N . U I D E S C         0 	        <?xml version="1.0" encoding="UTF-8"?>
<vstgui-ui-description version="1">
	<template 
    background-color="~ BlackCColor" 
    class="CViewContainer" 
    mouse-enabled="true" 
    name="view"
    origin="0, 0" size="300, 240">
	<view 
      class="CKnob" origin="60, 90" size="80, 80" 
      control-tag="Gain" 
      mouse-enabled="true" 
      corona-color="~ WhiteCColor" corona-inset="2" corona-outline="true" handle-color="~ RedCColor" handle-line-width="1" 
      />
		<view 
      class="CTextLabel" origin="60, 70" size="80, 20" 
      title="Gain"
      back-color="~ BlackCColor" font="~ SystemFont" font-color="~ WhiteCColor" text-alignment="center" />
		<view 
      class="CTextLabel" origin="60, 180" size="60, 20" 
      control-tag="Gain" 
      back-color="~ BlackCColor" font="~ SystemFont" font-color="~ WhiteCColor" text-alignment="center" />
		<view
      class="CTextLabel" origin="120, 180" size="20, 20" 
      title="dB"
      back-color="~ BlackCColor" font="~ SystemFont"  font-color="~ WhiteCColor" text-alignment="center" 
      />
	</template>
	<control-tags>
		<control-tag name="Gain" tag="0"/>
	</control-tags>
	<bitmaps/>
</vstgui-ui-description>
   