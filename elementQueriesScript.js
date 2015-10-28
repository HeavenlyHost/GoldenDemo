"use strict";

class ResizerDemo {

    constructor(container) {
        this.y = null;
        this.container = container;
        this.setupLayout();
    };

    static setupLayout() {
        this.handler = new Element('div', {
            'class': 'resizerDemo-handler'
        }).inject(this.container);

        this.container.makeResizable({
            snap: 0,
            handle: this.handler,
            modifiers: {
                'x': 'width',
                'y': this.y
            }
        });
    }
};

window.addEvent('domready', function () {
    $$('.examplesResizerDemos').each(function (resizer) {
        new ResizerDemo(resizer);
    });
});