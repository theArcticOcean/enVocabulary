import QtQuick 2.7 
import QtQuick.Controls 1.4 
import QtQuick.Window 2.2

// if we use window as root element, we need QQmlApplicationEngine.
Rectangle{
	visible:true
	width:150
	height:400
	Rectangle{
		x:0
		y:0
		width:150
		height:400
		clip:true	//if child rectangle's size is greater than it, clip it.

		Rectangle{
			x:0
			y:0
			width:150
			height:50
			visible:true
			color:"#AAFFAA"
			Text {
				anchors.centerIn: parent;
				text:"例句"
				color: "black";
				font.pixelSize: 32;
			}
		}

		Rectangle{
			x:0
			y:50
			width:75
			height:350
			visible:true
			color:"#FFAAAA"
			clip:true
			Text{
				anchors.centerIn: parent;
				text:"个人中心";
                width:35;       //this make sure one word every line.
                color:"black";
				font.pixelSize: 32;
                verticalAlignment:Text.AlignTop;
                wrapMode:Text.WordWrap;
			}
		}

        Rectangle{
            x:75;
            y:50;
            width:75;
            height:175;
            visible:true;
            color:"#AAAAFF";
            clip:true;
            Text{
                anchors.centerIn: parent;
                text:"查词历史";
                width:35;
                color:"black";
                font.pixelSize: 32;
                verticalAlignment: Text.AlignTop;
                wrapMode: Text.WordWrap;
            }
        }

        Rectangle {
            x:75;
            y:225;
            width:75;
            height:175;
            visible:true;
            color:"#AAFFFF";
            clip:true;
            Text{
                anchors.centerIn: parent;
                text:"收藏例句";
                width:35;
                color:"black";
                font.pixelSize: 32;
                verticalAlignment: Text.AlignTop;
                wrapMode: Text.WordWrap;
            }
        }
	}
}
