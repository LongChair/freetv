import QtQuick 1.0

Rectangle
{
   id : main
   width : 1920
   height : 1080
   color : "transparent"


   Component {
       id: channelItemDelegate
       Item {
           width: 180; height: 40
           Column {
               Text { text: '<b>Name:</b> '  }
               Text { text: '<b>Number:</b> '  }
           }
       }
   }

   ListView {
       id : channellist
       width: 300
       height: 500
       model : channelModel
       anchors.fill: parent
       opacity : 127
       focus : true
       keyNavigationWraps : true

       delegate : channelItemDelegate

//       delegate: Rectangle {
//                height: 25
//                width : 300
//                //color : ListView.isCurrentItem ?  "#40FF0000" : "#400000FF"
//                Text { text: modelData }

//            }

       highlight: Rectangle { color: "lightsteelblue"; radius: 5 }

       onCurrentIndexChanged:
       {
           console.debug("index changed");
       }

//       delegate: FocusScope {
//                       width: childrenRect.width; height: childrenRect.height
//                       x:childrenRect.x; y: childrenRect.y
//                       TextInput {
//                           focus: true
//                           text: modelData
//                           Keys.onReturnPressed: console.log(modelData)
//                       }
 //              }
   }

}


