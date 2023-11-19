document.querySelectorAll('hint').forEach((hint) => {
    console.log("setting eventlistener for " + hint);
    hint.addEventListener('click', (event) => {
        event.preventDefault();
        alert(hint.getAttribute('lore') === null ? 'No info here, sorry.' : hint.getAttribute('lore'));
    });
});