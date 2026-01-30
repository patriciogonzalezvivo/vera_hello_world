import './wasm-loader.js';

document.addEventListener('DOMContentLoaded', () => {
    const btn = document.getElementById('resize-btn');
    if (!btn) return;

    const wrapper = document.getElementById('wrapper');
    let isFullscreen = wrapper.classList.contains('fullscreen');

    btn.addEventListener('click', () => {
        isFullscreen = !isFullscreen;

        if (isFullscreen) {
            wrapper.classList.add('fullscreen');
            wrapper.classList.remove('windowed');
            document.body.classList.remove('windowed-mode');
        } else {
            wrapper.classList.remove('fullscreen');
            wrapper.classList.add('windowed');
            document.body.classList.add('windowed-mode');
        }
    });

    // Use ResizeObserver to handle the CSS transition smoothly
    // This fires repeatedly as the element resizes during transition
    const resizeObserver = new ResizeObserver(() => {
        window.dispatchEvent(new Event('resize'));
    });
    resizeObserver.observe(wrapper);
});

githubEmbed('.code', {
"owner": "patriciogonzalezvivo",
"repo": "vera_hello_world",
"ref": "main",
"embed": [{
    "path": "src/main.cpp"
    }]
});
