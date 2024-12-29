declare type int = ( number | 0 );
declare type bool = boolean;


/**
 * 触发器原型
 */
declare enum trigger {
    auto = 0, //自动触发
    enter = 1, //玩家对话
    close = 2 //玩家靠近
}

/**
 * 游戏事件原型
 */

declare interface GameEvents{
    Events:{
        PosX:int,
        PosY:int,
        Image:int,
        Walkable:bool,
        Trigger:trigger;
        EventNodes:BasicEvent[]
    }[]
}

declare interface BasicEvent{
    EventCode:int,
    Flag:int
}