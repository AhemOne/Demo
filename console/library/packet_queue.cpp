//
//  packet_queue.cpp
//  Car
//
//  Created by Jesse on 1/11/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#include "packet_queue.hpp"

/*
struct packet_queue::item {
    item * _next;
    packet_t _data;
};

packet_queue::item * packet_queue::last() {
    item * last = _list;
    if (last != nullptr) while (last->_next != nullptr) last = last->_next;
    return last;
}

packet_queue::packet_queue() {
    _list = nullptr;
}

void packet_queue::put(char * identifier, char * payload) {
    item * put = new item;
    put->_next = nullptr;
    put->_data = packet(identifier, payload);
    if (last() == nullptr) _list = put;
    else last()->_next = put;
}

void packet_queue::put(packet data) {
    put(data.identifier(), data.payload());
}

packet packet_queue::get() {
    if (_list == nullptr) return packet();
    packet got(_list->_data.identifier(), _list->_data.payload());
    item * current = _list;
    _list = _list->_next;
    delete current;
    return got;
}
*/
