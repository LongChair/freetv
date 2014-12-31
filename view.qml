import QtQuick 1.0

Rectangle {
    width: 1920
    height: 1080
    color: "transparent"

    Rectangle
    {
        width: 300
        height: parent.height
        color: "#803E606F"
        focus : true

        FocusScope
        {
            id: mainView
            focus : true
            width: parent.width; height: parent.height

            ListView {
                id : listChannels
                width: parent.width
                height: parent.height
                model : channelList
                focus : true
                highlight: highlightbar
                highlightFollowsCurrentItem : true
                highlightMoveSpeed : 1000

                Keys.onReturnPressed:
                {
                    tvplayer.play(listChannels.currentIndex);
                }

                delegate: Item {
                    id : itemdelegate
                    height: 50
                    width: listChannels.width
                    Image {
                        id: channelImage
                        height: parent.height
                        source: "qrc:/images/" + tvplayer.canal(index) + ".png"
                        smooth: true
                        opacity: 127
                    }
                    Text
                    {
                        width: 300
                        height: parent.height
                        text: modelData
                        color : listChannels.currentIndex == index ? "white" : "black"
                        horizontalAlignment:  Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: listChannels.currentIndex == index ? true : false
                        scale : listChannels.currentIndex == index ? 1.1 : 1.0
                    }
                }

                Component {
                    id : highlightbar
                    Rectangle {
                        width : itemdelegate.width
                        height: itemdelegate.height
                        color : "white"
                        opacity: 0.2
                        radius: 10
                        z : 1

                    }
                }
            }


        }
    }

}
